#ifndef GAMERENDERER_hpp
#define GAMERENDERER_hpp

#include <memory>
#include <glad/glad.h>

#include "gl/Camera.h"
#include "gl/shaderClass.h"
#include "jx_engine/physics/BulletDebugDrawer.cpp"
#include "jx_engine/physics/PhysicsWorldSingleton.h"

class GameGLRenderer {

    public:
        GameGLRenderer(int winWidth, int winHeight, Camera* cam);

        BulletDebugDrawer* debugDrawer;

        GLint colorUniformLocation; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;

        void RenderPrep();
        std::shared_ptr<Shader> mainShader;

        PhysicsWorldSingleton *physicsWorld;

        void DebugRender();

    private:
        Camera* camera;


        float lightCtr = 0.0f;
};

#endif