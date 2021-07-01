# FROM ubuntu:bionic
FROM node:alpine
MAINTAINER Joon-Yong Lee "joonyong.lee@pnnl.gov"

# # Install any extra things we might need
# RUN apt-get update && apt-get install -y \
#   vim \
#   ssh \
#   sudo \
#   wget \
#   curl \
#   gnupg2 \
#   software-properties-common ;\
#   rm -rf /var/lib/apt/lists/*


# # configure the repository for the nodejs and yarn
# RUN curl -sL https://deb.nodesource.com/setup_12.x | bash -
# RUN curl --insecure -sS https://dl.yarnpkg.com/debian/pubkey.gpg | apt-key add -
# RUN echo "deb [trusted=yes] https://dl.yarnpkg.com/debian/ stable main" | tee /etc/apt/sources.list.d/yarn.list

# # install python3
# RUN apt-get update && apt-get install -y nodejs yarn \
#   && rm -rf /var/lib/apt/lists/*

# app directory
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY package.json /usr/src/app
COPY yarn.lock /usr/src/app

RUN npm config set strict-ssl false
RUN npm install


# CMD ["yarn", "start"]