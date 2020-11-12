# Woxel
Woxel is an educational attempt at making a voxel-based game engine and game in C++.


## Getting started

Clone the repository with `--recursive`.

Compile vcpkg and install the dependancies. In the root folder, type:
```
.\vcpkg\bootstrap-vcpkg.bat -disableMetricsc        (or .sh if you're on a unix platform)
.\vcpkg\vcpkg install fmt spdlog glfw3 glad imgui[glfw-binding] taskflow entt glm
```
