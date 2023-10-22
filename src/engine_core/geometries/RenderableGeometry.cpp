#include "RenderableGeometry.h"

RenderableGeometry::RenderableGeometry(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
: 
    vbo(vboPtr), 
    vao(vaoPtr),
    ebo(eboPtr)
{
    _indices = indices;
    
    printf("Amount of triangles: %zu", indices.size());

    // Use the dereference operator (*) to access members of objects through their pointers.
    vao->Bind();

    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    vao->LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    
    vao->Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

RenderableGeometry::~RenderableGeometry()
{
    vao->Delete();
    vbo->Delete();
    ebo->Delete();
}

void RenderableGeometry::Draw(GLuint modelMatrixLocation, glm::mat4& modelMatrix)
{
    vao->Bind();
    //glBindTexture(GL_TEXTURE_2D, carTexID);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * _indices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);
}
