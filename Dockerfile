FROM ubuntu:23.10

RUN apt-get update
RUN apt-get -y install gcc g++ python3.11 bash
RUN ln -s /usr/bin/python3.11 /usr/bin/python
RUN apt-get -y install python3-pip python3.11-venv


RUN apt-get -y install pipx
RUN pip install --break-system-packages conan

RUN conan profile detect 

WORKDIR /root

COPY cmake/ .
COPY common/ .
COPY cws-map/ .
COPY grpc-server/ .
COPY proto/ .
COPY LICENSE ./
