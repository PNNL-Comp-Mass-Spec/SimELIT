import time
from backend import config
from backend.simulate import run
from backend.simulate import command
from backend.case import CaseFolder
from backend.data import DataStructure
from flask import Flask, jsonify, request, session
from flask_cors import CORS
from flask_socketio import SocketIO, emit, join_room
import uuid
import os

import subprocess

# Your API definition
app = Flask(__name__)
app.config.from_object(config.CeleryConfig())
# cors = CORS(app)
app.secret_key = os.urandom(24)
# app.secret_key="openfoam-key"

# socketio = SocketIO(app, cors_allowed_origins="*")
socketio = SocketIO(app, message_queue='redis://redis:6379/0', cors_allowed_origins="*")

@socketio.on('connect', namespace='/openfoam')
def socket_connect():
    print("CONNECTED")
    emit('ack', {"data": 'Well connected!'});

@socketio.on('join_room', namespace='/openfoam')
def on_room():
    # room = str(session['sid'])
    room = 1111
    print('join room: {}'.format(room))
    join_room(room)

# @app.route('/')
# def index():
#     if 'sid' not in session:
#         session['sid'] = str(uuid.uuid4())
#     return  { 'task_id': 0, 'queue_state': 0 }

@app.route('/init')
def init():
    # if 'sid' not in session:
    #     session['sid'] = str(uuid.uuid4())
    return  { 'task_id': 0, 'queue_state': 0 }

@app.route('/start', methods=['POST'])
def start():
    req_data = request.get_json()
    # sid=str(session["sid"])
    sid = 1111
    task = run.delay(config.GlobalConfig.SIMULATION_LOG, session=sid,
        solver=req_data['solver'], case=req_data['case'])
    return  { 'task_id': task.id, 'queue_state': task.state }

@app.route('/command', methods=['POST'])
def run_command():
    req_data = request.get_json()
    sid = 1111
    task = command.delay(config.GlobalConfig.SIMULATION_LOG, session=sid, command=req_data['command'])
    return  { 'task_id': task.id, 'queue_state': task.state }
    
@app.route('/status', methods=['POST'])
def taskstatus():
    print(request)
    req_data = request.get_json()
    task_id = req_data['task_id']
    print('task.id=', task_id)
    try:
        task = run.AsyncResult(task_id)
        if task.state is ['PROGRESS', 'PENDING']:
            response = {
                'task_id': task_id,
                'queue_state': task.state,
                'status': 'Process is ongoing...'
            }
        else:
            response = {
                'task_id': task_id,
                'queue_state': task.state
            }
    except Exception as e:
        response = {
            "message":"fail to find the task with the given task_id:{}".format(task_id)
        }
    return {
              'task_id': task_id,
              'queue_state': task.info['status']
          }

@app.route('/stop', methods=['POST'])
def stop():
    req_data = request.get_json()
    task_id = req_data['task_id']
    try:
        task = run.AsyncResult(task_id)
        task.abort()
    except Exception as e:
        response = {
            "message":"fail to find the task with the given task_id:{}".format(task_id)
        }
    
    return { 'task_id': task_id, "queue_state": task.state }

@app.route('/datafolders', methods=['POST'])
def get_data():
    # Separate these since solvers are copied into the container
    ds_cases = DataStructure('/usr/src/temp/')
    ds_solvers = DataStructure('/usr/src/data/')
    resp = jsonify({"solvers":ds_solvers.solvers, "cases":ds_cases.cases})

    return resp

@app.route('/files', methods=['POST'])
def files():
    req_data = request.get_json()
    case_path = req_data['case']
    cf = CaseFolder.create('/usr/src/temp/' + case_path)
    fs = cf.get_filetree()
    resp = jsonify(fs)

    return resp

@app.route('/file', methods=['POST'])
def file():
    req_data = request.get_json()
    case_path = req_data['case']
    folder = req_data['folder']
    foamFile = req_data['file']

    caseFolder = CaseFolder.create('/usr/src/temp/'+case_path)
    foamFile = caseFolder.data[folder][foamFile]
    foamFile.read()
    data = foamFile.data

    return { 'data': data }

    
@app.route('/save', methods=['POST'])
def save():
    req_data = request.get_json()
    case_path = req_data['case']
    folder = req_data['folder']
    foamFile = req_data['file']
    data = req_data['data']

    caseFolder = CaseFolder.create('/usr/src/temp/'+case_path)
    foamFile = caseFolder.data[folder][foamFile]
    foamFile.data = data
    foamFile.write()

    return { }
