
class GlobalConfig(object):
    SIMULATION_LOG = '/usr/src/backend/logfile.txt'
    SERVER_SLEEP = 1

class CeleryConfig(object):
    CELERY_BROKER_URL = 'redis://redis:6379/0'
    CELERY_RESULT_BACKEND = 'redis://redis:6379/0'