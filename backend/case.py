import os.path
from os import listdir
from PyFoam.RunDictionary.ParsedParameterFile import ParsedParameterFile

class CaseFolder:
    def __init__(self, filePath, data):
        self.filePath = filePath
        self.data = data

    def get_filetree(self):
        fileTree = dict()

        for key in self.data:
            dirElement = dict()
            dirElement['name'] = key
            dirElement['id'] = key
            dirElement['children'] = []
            fileTree[key] = dirElement
            foamMap = self.data[key]
            for fileKey in foamMap:
                fileElement = dict()
                fileElement['name'] = fileKey
                fileElement['id'] = key + "/" + fileKey
                fileElement['parent'] = key
                fileElement['filedata'] = ''
                fileTree[key]['children'].append(fileElement)
        
        return list(fileTree.values())

    @staticmethod
    def get_all_foamfiles(casefolder: str, casedirs: list):
        structure = dict()

        for directory in casedirs:
            if directory not in structure:
                structure[directory] = dict()

            fullDir = os.path.join(casefolder, directory)
            for f in [f for f in os.listdir(fullDir) if os.path.isfile(os.path.join(fullDir, f))]:
                # try:
                    foamFile = FoamFile(os.path.join(fullDir, f))
                    structure[directory][f] = foamFile

                # except Exception as e:
                #     print('unable to load {}'.format(f))

        return structure

    @staticmethod
    def create(filePath):
        data = CaseFolder.get_all_foamfiles(filePath, ["0", "system", "constant"])
        return CaseFolder(filePath, data)



class FoamFile:
    def __init__(self, filepath):
        self.filepath = filepath
        self.data = None
        cdir = os.path.split(filepath)
        self.foamFile = cdir[1]
        self.containingDirectory = cdir[0]

    def create(self, filepath):
        if filepath is None:
            raise Exception("must provide a filepath")
        
        self.data.writeFileAs(filepath)
        if os.path.isfile(filepath):
            return FoamFile(filepath)
        else:
            return None

    def read(self):
        # self.data = ParsedParameterFile(self.filepath)
        with open(self.filepath, 'r') as f:
            self.data = f.read()

    def write(self):
        # self.data.writeFile()
        with open(self.filepath, 'r+') as f:
            old = f.read()
            f.seek(0)
            f.write(self.data)
            f.truncate()

    # def get_parameter(self, param) -> str:
    #     if (self.data is not None):
    #         return self.data[param]
    #     return None

    # def set_parameter(self, param: str, value: str):
    #     if (self.data is not None):
    #         self.data[param] = value

    # def to_dictionary(self) -> dict:
    #     data = dict()

    #     if self.data == None:
    #         self.read()

    #     for keys in self.data:
    #         data[keys] = str(self.data[keys])

    #     return data


def get_file_structure(casefolder: str) -> list:
    fileTree = dict()
    caseDirs = ["0", "system", "constant"]

    for directory in caseDirs:
        fullDir = os.path.join(casefolder, directory)
        dirElement = dict()
        dirElement['name'] = directory
        dirElement['children'] = []
        fileTree[directory] = dirElement

        for f in [f for f in os.listdir(fullDir) if os.path.isfile(os.path.join(fullDir, f))]:
            fileElement = dict()
            fileElement['name'] = f
            fileTree[directory]['children'].append(fileElement)

    return list(fileTree.values())