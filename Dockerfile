FROM ubuntu:23.10

RUN apt-get update
RUN apt-get -y install gcc g++ bash cmake python3.11 python3-pip

RUN pip install --break-system-packages conan

RUN conan profile detect 

RUN mkdir -p /app
WORKDIR /app

# Mount project directory and run container
