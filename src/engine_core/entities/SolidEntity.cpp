#include "SolidEntity.h"


SolidEntity::SolidEntity(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) :
    geom(vaoPtr, vboPtr, eboPtr, vertices, indices)
 {
    std::cout << "RAN SOLID ENTITY CONSTR" << std::endl;
}