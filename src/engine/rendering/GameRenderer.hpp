#ifndef GAMERENDERER_hpp
#define GAMERENDERER_hpp

#include <memory>
#include <glad/glad.h>

#include "RenderRsrcManager.hpp"

#include "core/gl/Camera.h"
#include "core/gl/shaderClass.h"
#include "engine/SimulationWorld.hpp"

#include "core/gl/BulletDebugDrawer.cpp"
#include "ObjectRenderer.hpp"

class GameRenderer {

    public:
        GameRenderer(int winWidth, int winHeight, Camera* cam, SimulationWorld* world);
        void RenderALL();

        BulletDebugDrawer* debugDrawer;

    private:

        RenderRsrcManager ressources;

        Camera* camera;
        SimulationWorld* world;

        std::shared_ptr<Shader> mainShader;
        std::unique_ptr<ObjectRenderer> objectRender;

        
        GLint colorUniformLocation; //Debug purposes, might move

        GLint modelMatrixLOC;
        GLint useTextureLOC;



        void renderObjects();
        RenderList createObjectRenderList();

};

#endif