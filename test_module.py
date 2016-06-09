class test_class:
    def AddRef(self):
        print("PY: AddRef")
    def Release(self):
        print("PY: Release")

def func():
    print("PY: func")
    return test_class()
