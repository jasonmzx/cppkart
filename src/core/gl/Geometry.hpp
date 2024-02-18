#ifndef GEOMETRY_CLASS_H
#define GEOMETRY_CLASS_H

//*SOURCE Imports:

#include<glad/glad.h>
#include <vector>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

class Geometry
{
public:
    Geometry(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
    ~Geometry();

    VAO* vao;
    VBO* vbo;
    EBO* ebo;
    
    void Draw(GLuint modelMatrixLocation, glm::mat4& modelMatrix, GLuint colorUniformLocation, bool debugTriangles);

private:
    std::vector<GLuint> _indices; // Storing indices for use in the Draw function
};

#endif
