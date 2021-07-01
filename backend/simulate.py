# import config
from celery import Celery
from flask_socketio import SocketIO
import subprocess, select
from celery.contrib.abortable import AbortableTask
import os
import signal

class CeleryConfig(object):
    CELERY_BROKER_URL = 'redis://redis:6379/0'
    CELERY_RESULT_BACKEND = 'redis://redis:6379/0'

# Initialize Celery
celery = Celery(
    'worker', 
    broker=CeleryConfig.CELERY_BROKER_URL,
    backend=CeleryConfig.CELERY_RESULT_BACKEND
)

# socketio = SocketIO(message_queue='redis://redis:6379/0')
socketio = SocketIO(message_queue='redis://redis:6379/0', cors_allowed_origins="*")

def send_message(event, namespace, room, msg, source):
    print(msg)
    socketio.emit(event, {'msg':msg, 'source':source }, namespace=namespace, room=room)

@celery.task(bind=True, name='backend.simulate.run', base=AbortableTask)
def run(self, outpath, session, solver, case):
    room = session
    namespace = '/openfoam'
    # /home/foam/OpenFOAM/-7/platforms/linux64GccDPInt32Opt/bin/BGKionFoam
    proc = subprocess.Popen(['/bin/bash', '-c', '{0} -case /usr/src/temp/{1}'.format(solver, case)],
                            bufsize=8192,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            preexec_fn=os.setsid)
    
    send_message('status', namespace, room, 'Begin', 1)

    with open(outpath, "wb") as outf:
        dataend = False
        
        print("proc.returncode:", proc.returncode, "dataend:", dataend)

        while (proc.returncode is None) or (not dataend):
            proc.poll()
            dataend = False

            ready = select.select([proc.stdout, proc.stderr], [], [], 1.0)

            if proc.stderr in ready[0]:
                data = proc.stderr.read(1024)
                print(data)
                if len(data) > 0:
                    outf.write(data)
                    send_message('status', namespace, room, data.decode("utf-8"), 0)
                    self.update_state(state='FAILED',
                                             meta={
                                                'current': 0, 'total': 100,
                                                'status': data.decode("utf-8")
                                             })
                    return {'current': 100, 'total': 100, 'status': 'FAILED', 'result': 42}

            if proc.stdout in ready[0]:
                data = proc.stdout.read(1024)
                if len(data) == 0: # Read of zero bytes means EOF
                    dataend = True
                elif self.is_aborted():
                    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
                    send_message('status', namespace, room, os.linesep + 'Stopping', 1)
                    return { 'current': 0, 'total': 100, 'status': 'ABORTED', 'result': 42 }
                else:
                    outf.write(data)
                    send_message('status', namespace, room, data.decode("utf-8"), 2)
                    self.update_state(state='PROGRESS',
                                      meta={
                                          'current': 0, 'total': 100,
                                          'status': data.decode("utf-8")
                                      })
    
    send_message('status', namespace, room, 'End', 1)

    return {'current': 100, 'total': 100, 'status': 'COMPLETED',
            'result': 42}

@celery.task(bind=True, name='backend.simulate.command', base=AbortableTask)
def command(self, outpath, session, command):
    room = session
    namespace = '/openfoam'
    proc = subprocess.Popen(['/bin/bash', '-c', command],
                            bufsize=8192,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            preexec_fn=os.setsid)
    
    send_message('status', namespace, room, 'Begin', 1)

    with open(outpath, "wb") as outf:
        dataend = False
        while (proc.returncode is None) or (not dataend):
            proc.poll()
            dataend = False

            ready = select.select([proc.stdout, proc.stderr], [], [], 1.0)

            if proc.stderr in ready[0]:
                data = proc.stderr.read(1024)
                if len(data) > 0:
                    outf.write(data)
                    send_message('status', namespace, room, data.decode("utf-8"), 0)
                    self.update_state(state='FAILED',
                                             meta={
                                                'current': 0, 'total': 100,
                                                'status': data.decode("utf-8")
                                             })
                    return {'current': 100, 'total': 100, 'status': 'FAILED', 'result': 42}

            if proc.stdout in ready[0]:
                data = proc.stdout.read(1024)
                if len(data) == 0: # Read of zero bytes means EOF
                    dataend = True
                elif self.is_aborted():
                    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
                    send_message('status', namespace, room, os.linesep + 'Stopping', 1)
                    return { 'current': 0, 'total': 100, 'status': 'ABORTED', 'result': 42 }
                else:
                    outf.write(data)
                    send_message('status', namespace, room, data.decode("utf-8"), 2)
                    self.update_state(state='PROGRESS',
                                      meta={
                                          'current': 0, 'total': 100,
                                          'status': data.decode("utf-8")
                                      })
    
    send_message('status', namespace, room, 'End', 1)

    return {'current': 100, 'total': 100, 'status': 'COMPLETED',
            'result': 42}