cmake_minimum_required(VERSION 3.24)
project(cppkart)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)


include_directories(src/glad/include src)
add_executable(cppkart
src/glad/src/glad.c
src/stb/stb_image.h
src/stb/stb.cpp

src/main.cpp 
src/rendering/EBO.cpp 
src/rendering/VAO.cpp 
src/rendering/VBO.cpp 
src/rendering/shaderClass.cpp
src/rendering/Camera.cpp

src/rendering/Texture.h
src/rendering/Texture.cpp

)


target_link_libraries(cppkart PRIVATE SDL2::SDL2 OpenGL::GL)
