
# CPP Kart | (hopefully make it a real engine soon...)


## Setup:




### Setup for Debian-Like Linux: *(Debian, Ubuntu)*

1. Essential installs for C/C++ & CMake: <br/>
`sudo apt install build-essential cmake`

2. SDL Development Library Install: <br/>
`sudo apt install libsdl2-dev`

3. Assimp Loader & Utils Install: (Model Loader) <br/>
`sudo apt install assimp-utils libassimp-dev libassimp5`

4. Install Bullet Physics from VCPKG *(As Apt's latest version of bullet is not bullet3, and 2.7 has some issues lol)*

**Install VCPKG, then Bullet3**

```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install bullet3
```

*OpenGL Distribution comes natively with Kubuntu & Debian-Bookworm, if you don't have it, please install... *

Try: `sudo apt install libgl-dev mesa-utils` ?


### CMake & Build Executable:

1. Preliminary, for initial build: <br/>
`mkdir build && cd build`

2. Run CMake, then Make for building of executable: <br/>
`cmake ..` <br/>
`make`

3. Run it:
`./cppkart`

<br/>

---


**Tip:** in main directory (cppkart) you can just run `./DC_full_rebuild.sh` and it auto-deletes and re-C Makes and Makes the project.

---

### Some Useful Docs I keep laying arround...

Dear ImGUI [Example](https://github.com/ocornut/imgui/wiki/Getting-Started#example-if-you-are-using-sdl2--openglwebgl) 
<br/>
Dear ImGUI | [Getting Started](https://github.com/ocornut/imgui/wiki/Getting-Started)
<br/>
I followed for SDL2 and OpenGL3+ source code : [example_sdl2_opengl3/main.cpp](https://github.com/ocornut/imgui/blob/master/examples/example_sdl2_opengl3/main.cpp) 