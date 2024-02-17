cmake_minimum_required(VERSION 3.22)
project(cppkart)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(assimp REQUIRED)


# External Source Libraries

include_directories(src/glad/include src ${BULLET_INCLUDE_DIRS} src/imgui src/imgui/backends)

# --- Allows CMake to know all Imgui source files & backend impls I'm using
file(GLOB IMGUI_SOURCES 
     src/imgui/*.cpp 
     src/imgui/backends/imgui_impl_sdl2.cpp 
     src/imgui/backends/imgui_impl_opengl3.cpp)

add_executable(cppkart
     ${IMGUI_SOURCES}
     src/glad/src/glad.c
     src/stb/stb_image.h
     src/stb/stb.cpp

#    ######## Project Source Code: ########

      src/main.cpp 
      src/game/GameWindow.cpp
      src/game/GameWindow.hpp

      src/game/JXGame.cpp
      src/game/JXGame.hpp
      
     ##### Core:

          # Rendering:
          src/core/gl/EBO.cpp 
          src/core/gl/VAO.cpp 
          src/core/gl/VBO.cpp 
          src/core/gl/shaderClass.cpp
          src/core/gl/Camera.cpp

          src/core/gl/Texture.h
          src/core/gl/Texture.cpp

          # Loaders:
          
          src/core/loaders/ObjModel.cpp
          src/core/loaders/ObjModel.hpp

     
      ##### Engine:

     src/engine/objects/SimulationObject.cpp
     src/engine/objects/SimulationObject.hpp

     src/engine/objects/VehicleObject.cpp
     src/engine/objects/VehicleObject.hpp
     
     src/engine/SimulationWorld.cpp
     src/engine/SimulationWorld.hpp

     src/engine/physics/PhysicsWorldSingleton.cpp
     src/engine/physics/PhysicsWorldSingleton.h

     


#    --------------------- Old imports below 

#      #Core Engine Imports:
#      src/engine_core/XJZoomEngine.h
#      src/engine_core/XJZoomEngine.cpp

#      src/engine_core/PhysicsThread.h
#      src/engine_core/PhysicsThread.cpp

#      #Core Engine; Managers Imports:

#      src/engine_core/singletons/PhysicsWorldSingleton.h
#      src/engine_core/singletons/PhysicsWorldSingleton.cpp

#      ### Bullet Physics Models for Entites

#      src/engine_core/bullet_models/VehiclePhysics.h
#      src/engine_core/bullet_models/VehiclePhysics.cpp

#      src/engine_core/bullet_models/TerrainPhysics.h
#      src/engine_core/bullet_models/TerrainPhysics.cpp

#      src/engine_core/bullet_models/StaticTriangleMeshPhysics.h
#      src/engine_core/bullet_models/StaticTriangleMeshPhysics.cpp

#      ### Physics Chunking Manager

#      src/engine_core/managers/PhysicsChunkManager.h
#      src/engine_core/managers/PhysicsChunkManager.cpp

#      ## Bullet Debug Rendering

#      src/engine_core/geometries/BulletDebugDrawer.cpp


#      src/engine_core/managers/SceneManager.h
#      src/engine_core/managers/SceneManager.cpp

#      ## GL Renderable classes
#      src/engine_core/geometries/RenderableGeometry.h
#      src/engine_core/geometries/RenderableGeometry.cpp

#      ## Obj Model Loaders
#      src/engine_core/loaders/ObjModel.h
#      src/engine_core/loaders/ObjModel.cpp

#      #Terrain / Map Generator & Chunking Imports:
#      src/terrain_gen/MapChunker.h
#      src/terrain_gen/MapChunker.cpp

#      #Entities
#      src/engine_core/entities/VehicleEntity.h 
#      src/engine_core/entities/VehicleEntity.cpp

#      src/engine_core/entities/SolidEntity.h
#      src/engine_core/entities/SolidEntity.cpp

#      ##Renderers

#      src/engine_core/renderers/TerrainRenderer.h
#      src/engine_core/renderers/TerrainRenderer.cpp

#      # Main Imports for Rendering

#      src/main.cpp 
#      src/core/gl/EBO.cpp 
#      src/core/gl/VAO.cpp 
#      src/core/gl/VBO.cpp 
#      src/core/gl/shaderClass.cpp
#      src/core/gl/Camera.cpp

#      src/core/gl/Texture.h
#      src/core/gl/Texture.cpp

#      ## Data structs
#      src/data_structs/TSQueue.cpp

    



     
)

target_compile_options(cppkart PRIVATE -fsanitize=address)
target_link_libraries(cppkart PRIVATE -fsanitize=address)

# I'm assuming SDL2_mixer is already installed, FIX THIS!

target_link_libraries(cppkart PRIVATE SDL2_mixer SDL2::SDL2 OpenGL::GL ${BULLET_LIBRARIES} assimp::assimp)
