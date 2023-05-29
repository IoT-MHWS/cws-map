# cws-map
Coworking space map

## Build and install dependencies

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

Build image:

```bash
docker build -t cws-map-builder:latest .
```

Start container with bind-mounted directory:

```bash
docker run -d \
    -it \
    --name cws-map-dev \
    --mount type=bind,source="$(pwd)",target=/app \
    cws-map-builder:latest
```

Now you can attach to it:

```bash
docker exec -it cws-map-dev bash
```

Install dependencies and build program. I hope you won't accidentally remove container.

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

* [ ] Remove rtti
