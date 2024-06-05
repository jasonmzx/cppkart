#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include <string>
#include <memory>
#include <glad/glad.h>

#include "EComponent.hpp"

#include "jx_engine/render/RenderRsrcManager.hpp"
#include "jx_engine/render/gl/Geometry.hpp"


class RenderComponent : public EComponent {
    public:
        RenderComponent(std::string modelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm);

        void SetGLContext(GLint useTextureLOC, GLint modelMatrixLOC, GLint colorUniformLocation);
        void Draw();

    protected:
        std::shared_ptr<Geometry> Geom;
        std::string modelPath;
        std::string texPath;

        std::shared_ptr<RenderRsrcManager> ressources;

        std::shared_ptr<Texture> Tex;

        GLint colorUniformLocation; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;
        
        glm::mat4 modelMatrix;
};

#endif // RENDERCOMPONENT_HPP