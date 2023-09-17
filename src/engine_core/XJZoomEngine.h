#ifndef XJZOOMENGINE_CLASS_H
#define XJZOOMENGINE_CLASS_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "rendering/shaderClass.h"
#include "rendering/VAO.h"
#include "rendering/VBO.h"
#include "rendering/EBO.h"
#include "rendering/Camera.h"
#include "rendering/Texture.h"
#include "rendering/FrustumCull.h"

#include "terrain_gen/MapChunker.h"

//CORE Imports:
#include "engine_core/managers/SceneManager.h"

//For Texturing:
#include<stb/stb_image.h>


class XJZoomEngine {

public:
    void Run();
    void Init();
};

#endif