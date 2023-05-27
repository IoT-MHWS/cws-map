# cws-map
Coworking space map

## Install dependencies

Install `>=conan-2.0.0`.

Create conan profile:

```bash
conan profile detect [--force]
```

Install dependencies:

```bash
conan install . --build=missing
```

To not use lock file `--lockfile=""`.

To use different lock file `--lockfile=<path>`.

## Build

If using `>=cmake-3.25`, that implements presets, run:

```bash
cmake --preset conan-release
cmake --build --preset conan-release
```

Or read logs of previous command.

>  Modules can be run as project root modules, but need to add dependencies: check `conanfile.py` and conan documentation.

## Execute

Check executables in `build` folder.

## Docker

Build and run container:

```bash
docker build -t <name> .
docker run --rm -it <container-id> bash
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

