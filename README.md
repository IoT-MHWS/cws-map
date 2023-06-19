# cws-map

Coworking space map

## Before build

* Add proto repository in `/common/proto` directory. `conan create` doesn't handle symlinks, but `conan install` could be done.

## Build and install

Install `>=conan-2.0.0`.

Create conan profile:

```bash
conan profile detect [--force]
```

Install dependencies:

```bash
conan install . --build=missing [--settings=build_type=Debug]
```

If using `>=cmake-3.25`, that implements presets, run:

```bash
cmake --preset conan-release
cmake --build --preset conan-release
```

Or read logs of previous command.

>  Modules can be also used as project root modules, but need to add dependencies: check `conanfile.py` and conan documentation.

## Execute

Check executables in `build` folder.

## Docker

### Using image from dockerhub

Example using debug version. Create a new image from `Dockerfile.debug`:

```bash
docker build -t cws-map-app:debug-latest -f Dockerfile.debug .
```

### Creating images step by step locally

Create base image

```bash
docker build -t cws-map-base:latest -f Dockerfile.base .
```

Create image with installed libraries without using builder image from dockerhub. 

Run docker container:

```bash
docker run -d \
    -it \
    --name cws-map-builder-debug \
    --mount type=bind,source="$(pwd)",target=/app \
    cws-map-base:latest
```

Attach to running container and compile libraries using `conanfile.py` and `conan.lock`:

```bash
docker exec -t cws-map-builder-debug bash
# In container:
conan install . --build=missing --settings=build_type=Debug
```

Create snapshot of this image to use it to compile application:

```bash
docker commit cws-map-builder-debug cws-map-builder:debug-latest
```

Use this image to compile application. Can be done the same with mounts and running commands like in `Dockerfile.debug` or by creating a new image.

First of all, copy necessary dependencies from first section. Then run. Commands in container if using volumes:

```bash
conan install . --settings=build_type=Debug # should have all libraries installed
cmake --preset conan-debug # generate cmakefiles
cmake --build --preset conan-debug -j8 # build program
```

## CMake targets

Clean:

```bash
cmake --build --preset conan-release --target clean
```

Format:

```bash
cmake --build --preset conan-release --target=clang-format
cmake --build --preset conan-release --target=fix-clang-format
```

## TODO

