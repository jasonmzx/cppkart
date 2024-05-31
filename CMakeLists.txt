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

     ##### Game:

      src/game/GameWindow.cpp
      src/game/GameWindow.hpp

     #  src/game/JXGame.cpp
     #  src/game/JXGame.hpp

     src/game/state/GameInput.cpp
     src/game/state/GameInput.hpp

     src/game/debug/GameDebugHelper.cpp
     src/game/debug/GameDebugHelper.hpp

     ##### Core:

          src/core/gl/BulletDebugDrawer.cpp

          # Loaders:

          src/core/loaders/ChunkedMapLoader.cpp
          src/core/loaders/ChunkedMapLoader.hpp

          src/core/ID.hpp
          src/core/ID.cpp

     
      ##### Engine:

          # Objects:
          src/engine/objects/SimulationObject.cpp
          src/engine/objects/SimulationObject.hpp

          src/engine/objects/VehicleObject.cpp
          src/engine/objects/VehicleObject.hpp

          # Physics:
          src/engine/physics/PhysicsWorldSingleton.cpp
          src/engine/physics/PhysicsWorldSingleton.h

          src/engine/physics/VehiclePhysics.cpp
          src/engine/physics/VehiclePhysics.hpp

          src/engine/physics/StaticTriangleMeshPhysics.cpp
          src/engine/physics/StaticTriangleMeshPhysics.hpp

          # Physics Managers (Controllers):
          src/engine/physics/managers/PhysicsChunkManager.cpp 
          src/engine/physics/managers/PhysicsChunkManager.hpp



          # State:
          src/engine/state/GameInputState.hpp
     


     #!  ------------------------------ NEW FILES ------------------------------

     #---! Engine Rewrite Files !---:

     src/jx_engine/logs/Logger.hpp
     src/jx_engine/logs/Logger.cpp

     src/jx_engine/core/Scene.hpp
     src/jx_engine/core/Scene.cpp

     src/jx_engine/core/SceneManager.hpp
     src/jx_engine/core/SceneManager.cpp

     src/jx_engine/entity/ECManager.hpp
     src/jx_engine/entity/ECManager.cpp

     src/jx_engine/entity/Entity.hpp
     src/jx_engine/entity/Entity.cpp

     src/jx_engine/component/EComponent.hpp
     src/jx_engine/component/EComponent.cpp

     src/jx_engine/component/RenderComponent.hpp
     src/jx_engine/component/RenderComponent.cpp

     # Import / Loaders:

     src/jx_engine/import/ObjModel.cpp
     src/jx_engine/import/ObjModel.hpp

     # Rendering:

     src/jx_engine/render/GameGLRenderer.hpp
     src/jx_engine/render/GameGLRenderer.cpp

     src/jx_engine/render/RenderRsrcManager.hpp
     src/jx_engine/render/RenderRsrcManager.cpp

     src/jx_engine/render/gl/Geometry.cpp
     src/jx_engine/render/gl/Geometry.hpp

     src/jx_engine/render/gl/Texture.h
     src/jx_engine/render/gl/Texture.cpp

     src/jx_engine/render/gl/misc/ID.hpp
     src/jx_engine/render/gl/misc/ID.cpp

     src/jx_engine/render/gl/shaderClass.h
     src/jx_engine/render/gl/shaderClass.cpp

     src/jx_engine/render/gl/EBO.h
     src/jx_engine/render/gl/EBO.cpp

     src/jx_engine/render/gl/VBO.h
     src/jx_engine/render/gl/VBO.cpp

     src/jx_engine/render/gl/VAO.h
     src/jx_engine/render/gl/VAO.cpp

     src/jx_engine/render/gl/Camera.h
     src/jx_engine/render/gl/Camera.cpp


     #---! Rewrite Game Files !---:

     src/game/DemoGame.hpp
     src/game/DemoGame.cpp

     src/game/scenes/GameScene.hpp
     src/game/scenes/GameScene.cpp
)


# Enable AddressSanitizer
set(SANITIZE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")

target_compile_options(cppkart PRIVATE -fsanitize=address)
target_link_libraries(cppkart PRIVATE -fsanitize=address)

# I'm assuming SDL2_mixer is already installed, FIX THIS!

target_link_libraries(cppkart PRIVATE SDL2_mixer SDL2::SDL2 OpenGL::GL ${BULLET_LIBRARIES} assimp::assimp)
