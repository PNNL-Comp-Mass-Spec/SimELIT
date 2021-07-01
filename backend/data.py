import os.path
from os import listdir


FOLDERS_FOR_CASES = ["0", "system", "constant"]
FOLDERS_FOR_SOLVERS = ["Make"]


class DataStructure:
    def __init__(self, filePath):
        self.filePath = filePath
        self.solvers = {}
        self.cases = {}
        self.get_subdirs()

    def check_if_solver(self, subdirs):
        for i in FOLDERS_FOR_SOLVERS:
            if i not in subdirs:
                return False
        return True
        
    def check_if_case(self, subdirs):
        for i in FOLDERS_FOR_CASES:
            if i not in subdirs:
                return False
        return True

    def solver_exe(self, dir):
        print(dir)
        with open(os.path.join(dir, 'Make', 'files')) as f:
            line = f.readline()
            while line:
                if ("EXE" in line) & ("=" in line):
                    return line.strip().rsplit("/", 1)[1]
                line = f.readline()
        return ""
        

    def get_subdirs(self):
        subdirs = next(os.walk(self.filePath))[1]
        for d in subdirs:
            subdir = os.path.join(self.filePath, d)
            subsubdirs = next(os.walk(subdir))[1]
            if self.check_if_case(subsubdirs):
                self.cases[d] = subdir
            elif self.check_if_solver(subsubdirs):
                # self.solvers[d] = subdir
                self.solvers[d] = self.solver_exe(subdir)
                
