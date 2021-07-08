# Woxel
Woxel is an educational attempt at making a voxel-based game engine and game in C++.

## Getting started

Clone the repository with `--recursive`.

Compile vcpkg and install the dependancies. In the root folder, type:
```
.\vcpkg\bootstrap-vcpkg.bat -disableMetrics        (or .sh if you're on a unix platform)
.\vcpkg\vcpkg install fmt spdlog glfw3 glad imgui[docking-experimental,glfw-binding] taskflow entt glm
.\vcpkg\vcpkg install --head corrade[*] magnum[wglcontext] magnum-integration[glm]
```
