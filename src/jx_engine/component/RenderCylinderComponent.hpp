#ifndef RENDERCYLINDERCOMPONENT_HPP
#define RENDERCYLINDERCOMPONENT_HPP

#include "jx_engine/render/GameGLRenderer.hpp"

#include "EComponent.hpp"
#include "RenderComponent.hpp"

class RenderCylinderComponent : public EComponent {
    public:
        RenderCylinderComponent();
        ~RenderCylinderComponent();

        void tick() override;
        void Draw();
    protected:

        GLint colorUniformLOC; //Debug purposes, might move
        GLint modelMatrixLOC;
        GLint useTextureLOC;

        std::shared_ptr<Geometry> Geom;

        // Volatile GL Drawing Variables
        GLint currentCullFace;
        GLint currentFrontFace;
        
        glm::mat4 ObjmodelMatrix;
};


#endif // RENDERCYLINDERCOMPONENT_HPP