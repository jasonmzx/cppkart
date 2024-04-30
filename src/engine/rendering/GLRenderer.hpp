#ifndef GLRENDERER_hpp
#define GLERENDERER_hpp

#include <memory>
#include <glad/glad.h>

#include "core/gl/VAO.h"
#include "core/gl/Camera.h"

#include "core/gl/Geometry.hpp"
#include "core/gl/Texture.h"

struct RenderInstruction {
    uint64_t sortKey;
    glm::mat4 modelMatrix; //Still unsure if I should have this here

    std::shared_ptr<Geometry> geometry; // Use shared_ptr to Geometry
    
    std::shared_ptr<Texture> tex;
};

typedef std::vector<RenderInstruction> RenderList;

class GLRenderer {

    public:
        GLRenderer();

};

#endif