cmake_minimum_required(VERSION 3.24)
project(cppkart)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(assimp REQUIRED)

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

src/engine_core/bullet_models/LowAbsVehicle.h
src/engine_core/bullet_models/LowAbsVehicle.cpp

src/engine_core/bullet_models/TerrainPhysics.h
src/engine_core/bullet_models/TerrainPhysics.cpp

src/engine_core/bullet_models/SolidShapePhysics.h
src/engine_core/bullet_models/SolidShapePhysics.cpp

src/engine_core/bullet_models/StaticTriangleMeshPhysics.h
src/engine_core/bullet_models/StaticTriangleMeshPhysics.cpp

src/engine_core/managers/SceneManager.h
src/engine_core/managers/SceneManager.cpp

## GL Renderable classes
src/engine_core/geometries/RenderableGeometry.h
src/engine_core/geometries/RenderableGeometry.cpp

## Obj Model Loaders
src/engine_core/loaders/ObjModel.h
src/engine_core/loaders/ObjModel.cpp

#Terrain / Map Generator & Chunking Imports:
src/terrain_gen/MapChunker.h
src/terrain_gen/MapChunker.cpp

#Entities
src/engine_core/entities/VehicleEntity.h 
src/engine_core/entities/VehicleEntity.cpp

src/engine_core/entities/SolidEntity.h
src/engine_core/entities/SolidEntity.cpp

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

target_link_libraries(cppkart PRIVATE SDL2::SDL2 OpenGL::GL ${BULLET_LIBRARIES} assimp::assimp)
