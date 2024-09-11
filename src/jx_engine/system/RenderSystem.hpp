#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include <string>
#include <memory>
#include <glad/glad.h>
#include <chrono>

#include "System.hpp"

#include "jx_engine/render/RenderRsrcManager.hpp"
#include "jx_engine/render/GameGLRenderer.hpp"

#include "jx_engine/render/gl/Geometry.hpp"

#include "jx_engine/logs/Logger.hpp"


struct RenderComponent {
    
    std::string modelPath;
    std::shared_ptr<Geometry> Geom;

    std::string texPath;
    std::shared_ptr<Texture> Tex;

    glm::mat4 modelMatrix;
    
    bool cullDisable;

    GLint currentCullFace;
    GLint currentFrontFace;

};


class RenderSystem : public System {
    public:
        RenderSystem();
        RenderComponent buildRenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, float scale, bool cD, bool isTexAlpha);

        void SetGLContext(GLint texLOCATION, GLint mmLOCATION, GLint colorUniformLOCATION);

        void Draw(RenderComponent& rc);

        void tickSystem() override;

    protected:

        GLint colorUniformLOC; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;

        RenderRsrcManager* ressources;
        GameGLRenderer* renderer;
};

#endif // RENDERSYSTEM_HPP