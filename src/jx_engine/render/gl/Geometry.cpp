#include "Geometry.hpp"

Geometry::Geometry(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
{
    
    vao = std::make_shared<VAO>();
    vbo = std::make_shared<VBO>(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = std::make_shared<EBO>(indices.data(), indices.size() * sizeof(GLuint));
    
    _indices = indices;
    _vertices = vertices;
    
    std::cout << "Amount of triangles: " << indices.size() / 3 << "!\n";

    vao->Bind();

    // x y z r g b u v Nx Ny Nz     (11)
    // Each vertex consists of 11 floats: position (x, y, z), color (r, g, b), texture (u, v), normal (x, y, z)

    vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, VERT_LEN * sizeof(float), (void *)0);
    vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, VERT_LEN * sizeof(float), (void *)(3 * sizeof(float)));
    vao->LinkAttrib(*vbo, 2, 2, GL_FLOAT, VERT_LEN * sizeof(float), (void *)(6 * sizeof(float)));
    vao->LinkAttrib(*vbo, 3, 3, GL_FLOAT, VERT_LEN * sizeof(float), (void *)(8 * sizeof(float)));
    
    ebo->Bind(); // Bind EBO to VAO

    vao->Unbind();
}

void Geometry::Draw(GLuint modelMatrixLocation, glm::mat4& modelMatrix, GLuint colorUniformLocation, bool debugTriangles)
{

    vao->Bind();
    //glBindTexture(GL_TEXTURE_2D, carTexID);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * _indices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);

        if (debugTriangles) {
        // Save current polygon mode
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);

        // Set to wireframe mode to draw the triangle outlines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0f); // Set line width for wireframe, adjust as needed

        // Set a different color for the wireframe, e.g., red
        glUniform4f(colorUniformLocation, 1.0f, 0.0f, 0.0f, 1.0f); // Replace 'colorUniformLocation' with your color uniform location

        // Draw the triangles again as wireframe
        glDrawElements(GL_TRIANGLES, (sizeof(GLuint) * _indices.size()) / sizeof(int), GL_UNSIGNED_INT, 0);

        // Restore the original polygon mode
        glPolygonMode(GL_FRONT, polygonMode[0]);
        glPolygonMode(GL_BACK, polygonMode[1]);

        // Reset the color to the original one (if needed)
        }
}

std::vector<glm::vec3> Geometry::GetXYZvertices() // Return the vertices as a vector of glm::vec3 (For use in physics)
{
    
    std::vector<glm::vec3> ordered_vertices;

    for (int i = 0; i < _indices.size(); i++) {
        ordered_vertices.push_back(
            glm::vec3(_vertices[_indices[i] * VERT_LEN], _vertices[_indices[i] * VERT_LEN + 1], _vertices[_indices[i] * VERT_LEN + 2])
        );
    }
    
    return ordered_vertices;
    
    // std::vector<glm::vec3> vertices;
    // for (int i = 0; i < _vertices.size(); i += VERT_LEN) {
    //     vertices.push_back(glm::vec3(_vertices[i], _vertices[i + 1], _vertices[i + 2]));
    // }
    // return vertices;
}