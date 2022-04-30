# SimELIT

SimELIT is a novel ion trajectory simulation platform, which stands for the **Sim**ulator of **E**ulerian and **L**agrangian **I**on **T**rajectories. This provides a web-based GUI application to control and edit the OpenFOAM applications. 


# Requirements
- docker
- docker-compose


# How to Run

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


**note:** 7-Zip is recommended for unzipping the dowloaded repository as the Windows provided utility for unzipping may fail due to large path lengths. 


## Contacts ##
Written by Joon-Yong Lee and Cameron Giberson for the Department of Energy (PNNL, Richland, WA)\
Copyright 2021, Battelle Memorial Institute. All Rights Reserved.\
E-mail: joonyong.lee@pnnl.gov or cameron.giberson@pnnl.gov or proteomics@pnnl.gov\
Website: https://omics.pnl.gov/ or https://panomics.pnnl.gov/


## License ##
SimELIT is licensed under the BSD 2-Clause License; [License](license.txt)
