#ifndef XJZOOMENGINE_CLASS_H
#define XJZOOMENGINE_CLASS_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

//* ImGUI Imports:
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

//*SOURCE Imports:

#include "rendering/shaderClass.h"
#include "rendering/VAO.h"
#include "rendering/VBO.h"
#include "rendering/EBO.h"
#include "rendering/Camera.h"
#include "rendering/Texture.h"
#include "terrain_gen/MapChunker.h"

//Renderable geoms
#include "geometries/RenderableGeometry.h"

//Entity Imports

#include "engine_core/entities/VehicleEntity.h"
#include "engine_core/entities/SolidEntity.h"

// Physics stuff:
#include "singletons/PhysicsWorldSingleton.h"

#include "engine_core/bullet_models/TerrainPhysics.h" //TODO: Remove and wrap in a TerrainEntity class

#include "engine_core/geometries/BulletDebugDrawer.cpp"

//CORE Imports:
#include "engine_core/managers/SceneManager.h"

//For Texturing:
#include<stb/stb_image.h>

//Model Loader
#include "loaders/ObjModel.h"


//* ----- Terrain Imports -----
#include "engine_core/managers/PhysicsChunkManager.h"
#include "engine_core/renderers/TerrainRenderer.h"


#include "engine_core/PhysicsThread.h"
//* Vehicle Input Controller

#include "controllers/VehicleInputAdapter.h"

class XJZoomEngine {

public:
    void Run();
    void Init();
};

#endif