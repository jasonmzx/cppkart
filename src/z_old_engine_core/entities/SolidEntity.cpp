#include "SolidEntity.h"


SolidEntity::SolidEntity(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, glm::mat4& modelMatrix) 
    :
    geom(vaoPtr, vboPtr, eboPtr, vertices, indices),
    shapePhysics(vertices, indices, modelMatrix)
{
    std::cout << "RAN SOLID ENTITY CONSTR" << std::endl;
}