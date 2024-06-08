#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include <string>
#include <memory>
#include <glad/glad.h>

#include "EComponent.hpp"

#include "jx_engine/render/RenderRsrcManager.hpp"
#include "jx_engine/render/gl/Geometry.hpp"

#include "jx_engine/logs/Logger.hpp"


class RenderComponent : public EComponent {
    public:
        RenderComponent(std::string modelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm);

        void SetGLContext(GLint texLOCATION, GLint mmLOCATION, GLint colorUniformLOCATION);
        void Draw();

    protected:
        std::shared_ptr<Geometry> Geom;
        std::string modelPath;
        std::string texPath;

        std::shared_ptr<RenderRsrcManager> ressources;

        std::shared_ptr<Texture> Tex;

        GLint colorUniformLOC; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;
        
        glm::mat4 ObjmodelMatrix;
        
};

#endif // RENDERCOMPONENT_HPP