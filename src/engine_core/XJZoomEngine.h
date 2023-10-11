#ifndef XJZOOMENGINE_CLASS_H
#define XJZOOMENGINE_CLASS_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>



//*SOURCE Imports:

#include "rendering/shaderClass.h"
#include "rendering/VAO.h"
#include "rendering/VBO.h"
#include "rendering/EBO.h"
#include "rendering/Camera.h"
#include "rendering/Texture.h"
#include "rendering/FrustumCull.h"

#include "terrain_gen/MapChunker.h"

//Entity Imports

#include "engine_core/entities/VehicleEntity.h"


// Physics stuff:
#include "singletons/PhysicsWorldSingleton.h"

#include "engine_core/bullet_models/TerrainPhysics.h" //TODO: Remove and wrap in a TerrainEntity class

//CORE Imports:
#include "engine_core/managers/SceneManager.h"

//For Texturing:
#include<stb/stb_image.h>


//Model Loader
#include "loaders/ObjModel.h"

class XJZoomEngine {

public:
    void Run();
    void Init();
};

#endif