FROM ubuntu:bionic
MAINTAINER Joon-Yong Lee "joonyong.lee@pnnl.gov"

# Install any extra things we might need
RUN apt-get update && apt-get install -y \
  vim \
  ssh \
  sudo \
  wget \
  curl \
  # build-essential \
  # flex \
  # bison \
  # git-core \
  # cmake \
  # zlib1g-dev \
  # libboost-system-dev \
  # libboost-thread-dev \
  # libopenmpi-dev \
  # openmpi-bin \
  # gnuplot \
  # libreadline-dev \
  # libncurses-dev \
  # libxt-dev \
  software-properties-common ;\
  rm -rf /var/lib/apt/lists/*

# Install OpenFOAM v6 (without ParaView)
# plus an extra environment variable to make OpenMPI play nice
RUN sh -c "wget -O - http://dl.openfoam.org/gpg.key | apt-key add -" ;\
  add-apt-repository http://dl.openfoam.org/ubuntu ;\
  apt-get update ;\
  apt-get install -y --no-install-recommends openfoam7 ;\
  rm -rf /var/lib/apt/lists/* ;\
  echo "source /opt/openfoam7/etc/bashrc" >> ~/.bashrc ;\
  echo "export OMPI_MCA_btl_vader_single_copy_mechanism=none" >> ~/.bashrc

# configure the repository for the nodejs and yarn
RUN curl -sL https://deb.nodesource.com/setup_12.x | bash -
RUN curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | apt-key add -
RUN echo "deb https://dl.yarnpkg.com/debian/ stable main" | tee /etc/apt/sources.list.d/yarn.list

# install python3
RUN apt-get update && apt-get install -y python3.7 python3-pip nodejs yarn \
  && rm -rf /var/lib/apt/lists/*

# install packages
RUN pip3 install --user flask-restful pandas flask python-dotenv

ADD data/BGKionFoam /home/app/data/solver
ADD data/caseFolder /home/app/data/case
# ADD run.sh /home/app/run.sh
COPY my-app /home/app/

WORKDIR /home/app/


# CMD [ "/bin/bash" ]

# BGKionFoam
# RUN /opt/openfoam7/wmake/wmake data/solver

# RUN /usr/bin/node -v
# RUN /usr/bin/npm -v
# RUN /usr/bin/yarn start
# RUN /usr/bin/yarn start-api