FROM ubuntu:23.10

RUN apt-get update && apt-get install -y --fix-missing \
    gcc g++ bash cmake python3.11 python3-pip git

RUN pip install --break-system-packages conan

RUN conan profile detect 

RUN mkdir -p /app
WORKDIR /app

# Use this image to run `conan install` to create cws-map-builder images
