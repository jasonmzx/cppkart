#ifndef GAMERENDERER_hpp
#define GAMERENDERER_hpp

#include <memory>
#include <glad/glad.h>

#include "GLRenderer.hpp"

#include "core/gl/Camera.h"
#include "core/gl/shaderClass.h"

#include "engine/SimulationWorld.hpp"

class GameRenderer {

    public:
        GameRenderer(int winWidth, int winHeight, Camera* cam);
        void RenderALL(SimulationWorld* world);

    private:
        Camera* camera;
        void renderObjects(const SimulationWorld* world);
        RenderList createObjectRenderList(const SimulationWorld *world);

        std::unique_ptr<Shader> mainShader;

        GLint modelMatrixLOC;
        GLint useTextureLOC;

};

#endif