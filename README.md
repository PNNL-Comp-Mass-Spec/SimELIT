# OpenFOAM GUI

This is a web-based GUI application to control and edit the OpenFOAM applications. 


# Requirements
- docker
- docker-compose


# How to Run

First, please adjust the following absolute path for the data in docker-compose.yml.
```
/Users/leej324/git/openfoam-gui/data/BGKionFoam
/Users/leej324/git/openfoam-gui/data/caseFolder
```


1. build the docker images
```bash
make build
```

In Windows,
```bash
docker-compose build
```

2. run the openfoam-gui app
```bash
make run
```

In Windows,
```bash
docker-compose up
```



## Contacts ##
Written by Joon-Yong Lee and Cameron Giberson for the Department of Energy (PNNL, Richland, WA)\
Copyright 2021, Battelle Memorial Institute. All Rights Reserved.\
E-mail: joonyong.lee@pnnl.gov or cameron.giberson@pnnl.gov or proteomics@pnnl.gov\
Website: https://omics.pnl.gov/ or https://panomics.pnnl.gov/


## License ##
AutoCCS is licensed under the BSD 2-Clause License; [License](license.txt)
