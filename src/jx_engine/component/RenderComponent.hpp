#ifndef RENDERCOMPONENT_HPP
#define RENDERCOMPONENT_HPP

#include <string>
#include <memory>
#include <glad/glad.h>
#include <chrono>

#include "EComponent.hpp"

#include "jx_engine/render/RenderRsrcManager.hpp"
#include "jx_engine/render/GameGLRenderer.hpp"

#include "jx_engine/render/gl/Geometry.hpp"

#include "jx_engine/logs/Logger.hpp"


class RenderComponent : public EComponent {
    public:
        RenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, bool cD, bool isTexAlpha);

        std::vector<glm::vec3> GetXYZvertices();

        void SetGLContext(GLint texLOCATION, GLint mmLOCATION, GLint colorUniformLOCATION);
        void SetRenderScale(float scale);
        
        void CentreArroundXYZ(glm::vec3 position);

        void Draw();

        void tick() override;

    protected:
        std::shared_ptr<Geometry> Geom;
        std::string modelPath;
        std::string texPath;

        bool cullDisable;

        RenderRsrcManager* ressources;

        std::shared_ptr<Texture> Tex;

        GLint colorUniformLOC; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;
        
        
        // Volatile GL Drawing Variables
        GLint currentCullFace;
        GLint currentFrontFace;
        
        glm::mat4 ObjmodelMatrix;
        glm::mat4 IdentityScaleMatrix;
        
};

#endif // RENDERCOMPONENT_HPP