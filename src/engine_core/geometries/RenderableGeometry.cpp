#include "RenderableGeometry.h"

//Member Initialization of EBO (indices) & VBO (vertices), with the params passed over

RenderableGeometry::RenderableGeometry(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
: 
    vbo(vertices.data(), sizeof(GLfloat) * vertices.size()), 
    ebo(indices.data(), sizeof(GLuint) * indices.size())
{
    //! Nasty segfault happens when indices are loaded in..
    //_indices = indices;
    
    printf("Amount of triangles: %zu", indices.size());
    vao.Bind();

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();


}

RenderableGeometry::~RenderableGeometry()
{
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
}

void RenderableGeometry::Draw(GLuint modelMatrixLocation, glm::mat4& modelMatrix)
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // Handle the error or print it out
        printf("OpenGL error: %d\n", error);
    }

    vao.Bind();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * _indices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);
}
