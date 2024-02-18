#ifndef GAMERENDERER_hpp
#define GAMERENDERER_hpp

#include <memory>
#include <glad/glad.h>
#include <unordered_map> // For geometry caching

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
        std::unique_ptr<Shader> mainShader;
        
        GLint colorUniformLocation; //Debug purposes, might move

        GLint modelMatrixLOC;
        GLint useTextureLOC;

        //Caching Geoms. to avoid duplications of identical geometries
        std::unordered_map<std::string, std::shared_ptr<Geometry>> geometryCache;

        void renderObjects();
        RenderList createObjectRenderList();
        std::shared_ptr<Geometry> getOrCreateGeometry(const std::string& modelIdentifier, 
        const std::vector<GLfloat>& verts, const std::vector<GLuint>& indices);
};

#endif