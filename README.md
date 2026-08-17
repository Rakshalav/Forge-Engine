# FrameGraph Example


## Features
- To be added.


## Building

Requires [vcpkg](https://github.com/microsoft/vcpkg)

```bash
git clone https://github.com/Rakshalav/Forge-Engine.git
./scripts/Setup-Windows.bat
```

### vcpkg quickstart

https://github.com/microsoft/vcpkg#getting-started

```bash
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.bat
```

Add the following environment variables

```bash
VCPKG_ROOT = path_to_vcpkg
VCPKG_DEFAULT_TRIPLET = x64-windows
```

Install required dependencies

```bash
vcpkg install assimp:x64-windows-static efsw:x64-windows-static glad[gl-api-46]:x64-windows-static glad[loader]:x64-windows-static glfw3:x64-windows-static glm:x64-windows-static spdlog:x64-windows-static yaml-cpp:x64-windows-static
```

## Dependencies

- [assimp](https://github.com/assimp/assimp)
- [efsw](https://github.com/SpartanJ/efsw)
- [glad](https://glad.dav1d.de/)
- [GLFW](https://github.com/glfw/glfw)
- [glm](https://github.com/g-truc/glm)
- [ImGui](https://github.com/ocornut/imgui)
- OpenGL 4.6
- [spdlog](https://github.com/gabime/spdlog)
- [stb_image](https://github.com/nothings/stb)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)