# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

Coursework repository for a "Processamento Gráfico: Fundamentos" (Fundamentals of Computer Graphics) class. Each exercise/example is a standalone C++/OpenGL program built with CMake, using GLFW for windowing, GLAD for OpenGL function loading, and GLM for math.

## Environment

This repo uses Nix flakes (`flake.nix`) for the dev environment, activated automatically via direnv (`.envrc` — `use flake`). The shell provides `cmake`, `glfw`, `glm`, `libGL`, `gcc`, and exposes the vendored `stb` headers (single-header image libs) via `CPATH`.

If direnv isn't active, enter the shell manually with `nix develop`.

## Build

```sh
cmake -S . -B build
cmake --build build
```

Binaries are produced per-exercise in `build/` (e.g. `build/HelloTriangle`).

## Adding a new exercise

`CMakeLists.txt` builds one executable per entry in the `EXERCISES` list (around line 18). Each entry is a path relative to `src/`, and the executable name is the last path component, prefixed with the list folder when it starts with `Lista` (e.g. `Exercicios/Lista2/1` → `build/Lista2_1`, `Exemplos/HelloTriangle` → `build/HelloTriangle`):

```cmake
set(EXERCISES
    Exemplos/HelloTriangle
    Exercicios/Lista1/1
    Exercicios/Lista1/3
)
```

To add a new exercise: create `src/<Category>/<Name>/` containing its `.cpp` file(s), then add `<Category>/<Name>` to the `EXERCISES` list. All `.cpp` files in that directory are globbed automatically and linked against `glad.c`, `glfw`, `glm::glm`, and the platform OpenGL library. Avoid accents/special characters in directory names (enforced by convention, noted in the CMakeLists comment).

Note: `EXERCISES` currently lists `Exercicios/Lista1/1` and `Exercicios/Lista1/3`, which don't exist yet under `src/` — the build will fail until those directories are created or the entries are removed.

## GLAD / third-party code

- `include/glad/glad.h` and `include/glad/KHR/khrplatform.h` — GLAD headers (checked into the repo, not fetched by CMake).
- `common/glad.c` — GLAD loader implementation, compiled into every exercise executable.
