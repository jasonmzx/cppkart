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

        static GameGLRenderer* getInstance(int winWidth, int winHeight, Camera *cam);
        static GameGLRenderer* getInstance();

        GameGLRenderer(int winWidth, int winHeight, Camera* cam);

        BulletDebugDrawer* debugDrawer;

        GLint colorUniformLocation; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;

        void RenderPrep();
        std::shared_ptr<Shader> mainShader;

        PhysicsWorldSingleton *physicsWorld;

        void DebugRender();
        void DebugDrawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);

        void UpdateScreenSize(int winWidth, int winHeight);

        void changeShader(const std::string &vertexShaderPath);

        // ** Geometric Shape Gens **

        std::vector<GLfloat> GetCylinderVertices(int n_sides, float height);
        std::vector<GLuint> GetCylinderIndices(int n_sides);

        std::vector<GLfloat> GetConeVertices(int n_sides, float height);
        std::vector<GLuint> GetConeIndices(int n_sides);

    private:
        
        static GameGLRenderer* instance;
        
        Camera* camera;
    

        float lightCtr = 0.0f;
};

#endif