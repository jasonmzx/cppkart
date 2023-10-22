#ifndef RENDERABLEGEOMETRY_CLASS_H
#define RENDERABLEGEOMETRY_CLASS_H

//*SOURCE Imports:

#include<glad/glad.h>
#include <vector>

#include "rendering/shaderClass.h"
#include "rendering/VAO.h"
#include "rendering/VBO.h"
#include "rendering/EBO.h"
#include "rendering/Camera.h"

class RenderableGeometry
{
public:
    RenderableGeometry(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
    ~RenderableGeometry();

    VAO* vao;
    VBO* vbo;
    EBO* ebo;
    
    void Draw(GLuint modelMatrixLocation, glm::mat4& modelMatrix);

private:
    std::vector<GLuint> _indices; // Storing indices for use in the Draw function
};

#endif
