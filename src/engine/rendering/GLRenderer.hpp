#ifndef GLRENDERER_hpp
#define GLERENDERER_hpp

#include <memory>
#include <glad/glad.h>

#include "core/gl/VAO.h"
#include "core/gl/Camera.h"


struct RenderInstruction {
    uint64_t sortKey;
    glm::mat4 modelMatrix; //Still unsure if I should have this here

    //Some kind of Draw Patameters data structure

    VAO* VertexArrayObject;

};

typedef std::vector<RenderInstruction> RenderList;

class GLRenderer {

    public:
        GLRenderer();

};

#endif