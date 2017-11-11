class CodeWithInitialization(object):
    def __init__(self, code, initialization=None):
        self.code = code
        self.initialization = initialization or []
