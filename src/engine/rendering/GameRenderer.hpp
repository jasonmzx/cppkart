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
        GameRenderer(int winWidth, int winHeight, Camera* cam, SimulationWorld* world);
        void RenderALL();

    private:
        Camera* camera;
        SimulationWorld* world;

        void renderObjects();
        RenderList createObjectRenderList(const SimulationWorld *world);

        std::unique_ptr<Shader> mainShader;

        GLint modelMatrixLOC;
        GLint useTextureLOC;

        VAO VAO1;

};

#endif