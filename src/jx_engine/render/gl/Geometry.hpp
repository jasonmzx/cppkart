#ifndef GEOMETRY_CLASS_H
#define GEOMETRY_CLASS_H

//*SOURCE Imports:

#include<glad/glad.h>
#include <vector>
#include <memory>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

class Geometry
{
public:
    Geometry(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
    ~Geometry() = default; 

    int VERT_LEN = 11; // x y z r g b u v Nx Ny Nz     (11)

    void Draw(GLuint modelMatrixLocation, glm::mat4& modelMatrix, GLuint colorUniformLocation, bool debugTriangles);

    std::vector<GLfloat> _vertices;
    std::vector<GLuint> _indices; // Storing indices for use in the Draw function
    
    std::vector<glm::vec3> GetXYZvertices();

private:

    std::shared_ptr<VAO> vao;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<EBO> ebo;

    //std::vector<GLfloat> _vertices;
    //std::vector<GLuint> _indices; // Storing indices for use in the Draw function
};

#endif
