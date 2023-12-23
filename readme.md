build command i use:

`
/usr/bin/cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ -S/home/jason/GitHub/cppkart -B/home/jason/GitHub/cppkart/build -G Ninja
`

Install ASSIMP (Model/Asset loader)


### Setup for Debian-Like Linux: *(Debian, Ubuntu)*

1. Essential installs for C/C++ & CMake:
`sudo apt install build-essential cmake`

2. SDL Development Library Install:
`sudo apt install libsdl2-dev`

3. Bullet Physics Development Library Install:
`sudo apt install libbullet-dev`

4. Assimp Loader & Utils Install:
`sudo apt install assimp-utils libassimp-dev libassimp5`

*OpenGL Distribution comes natively with Kubuntu & Debian-Bookworm, if you don't have it, please install*

### CMake & Build Executable:

1. Preliminary, for initial build: <br/>
`mkdir build && cd build`

2. Run CMake, then Make for building of executable: <br/>
`cmake ..` <br/>
`make`

3. Run it:
`./cppkart`