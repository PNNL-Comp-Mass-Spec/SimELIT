FROM ubuntu:bionic as base
MAINTAINER Joon-Yong Lee "joonyong.lee@pnnl.gov"

# RUN rm /bin/sh && ln -s /bin/bash /bin/sh

# Install any extra things we might need
RUN apt-get update && apt-get install -y \
  vim \
  ssh \
  sudo \
  wget \
  curl \
  gnupg2 \
  software-properties-common ;\
  rm -rf /var/lib/apt/lists/*

# Create a new user called foam
RUN useradd --user-group --create-home --shell /bin/bash foam ;\
  echo "foam ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers


# Install OpenFOAM v7 (without ParaView)
# plus an extra environment variable to make OpenMPI play nice
RUN sh -c "wget -O - http://dl.openfoam.org/gpg.key | apt-key add -" ;\
  add-apt-repository http://dl.openfoam.org/ubuntu ;\
  apt-get update ;\
  apt-get install -y --no-install-recommends openfoam7 ;\
  rm -rf /var/lib/apt/lists/* ;\
  echo "source /opt/openfoam7/etc/bashrc" >> ~foam/.bashrc ;\
  echo "export OMPI_MCA_btl_vader_single_copy_mechanism=none" >> ~foam/.bashrc

# install python3
RUN apt-get update && apt-get install -y python3.7 python3-pip \
  && rm -rf /var/lib/apt/lists/*

COPY --chown=foam:foam /data /usr/src/data
RUN chmod -R 777 /usr/src/data

RUN mkdir -p /usr/src/backend
WORKDIR /usr/src/backend

COPY /backend/requirements.txt /usr/src/backend

# VOLUME /Users/leej324/git/notebooks/openfoam/data/BGKionFoam /usr/src/app/solver
# COPY install.sh /usr/src/app

# RUN /bin/bash -c "source ~/.bashrc && /opt/openfoam7/wmake/wmake solver"
# RUN source /opt/openfoam7/etc/bashrc && /opt/openfoam7/wmake/wmake /usr/src/app/solver

RUN pip3 install --trusted-host pypi.org --trusted-host pypi.python.org --trusted-host=files.pythonhosted.org --no-cache-dir -r requirements.txt

ENV OMPI_MCA_btl_vader_single_copy_mechanism none
ENV LC_ALL C.UTF-8
ENV LANG C.UTF-8

EXPOSE 5000
USER foam

FROM base
RUN ["/bin/bash", "-c", "source ~/.bashrc && source /opt/openfoam7/etc/bashrc && wclean all /usr/src/data && wmake all /usr/src/data"]