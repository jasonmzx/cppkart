cmake_minimum_required(VERSION 3.24)
project(cppkart)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)

# External Source Libraries

include_directories(src/glad/include src ${BULLET_INCLUDE_DIRS})
add_executable(cppkart
src/glad/src/glad.c
src/stb/stb_image.h
src/stb/stb.cpp

#### Project Source Code: ####

#Core Engine Imports:
src/engine_core/XJZoomEngine.h
src/engine_core/XJZoomEngine.cpp

#Core Engine; Managers Imports:

src/engine_core/singletons/PhysicsWorldSingleton.h
src/engine_core/singletons/PhysicsWorldSingleton.cpp

### Bullet Physics Models for Entites
src/engine_core/bullet_models/VehiclePhysics.h
src/engine_core/bullet_models/VehiclePhysics.cpp

src/engine_core/bullet_models/TerrainPhysics.h
src/engine_core/bullet_models/TerrainPhysics.cpp


src/engine_core/managers/SceneManager.h
src/engine_core/managers/SceneManager.cpp



#Terrain / Map Generator & Chunking Imports:
src/terrain_gen/MapChunker.h
src/terrain_gen/MapChunker.cpp

#Entities
src/engine_core/entities/VehicleEntity.h 
src/engine_core/entities/VehicleEntity.cpp


# Main Imports for Rendering

src/main.cpp 
src/rendering/EBO.cpp 
src/rendering/VAO.cpp 
src/rendering/VBO.cpp 
src/rendering/shaderClass.cpp
src/rendering/Camera.cpp

src/rendering/FrustumCull.h
src/rendering/FrustumCull.cpp

src/rendering/Texture.h
src/rendering/Texture.cpp

)

target_link_libraries(cppkart PRIVATE SDL2::SDL2 OpenGL::GL ${BULLET_LIBRARIES})
