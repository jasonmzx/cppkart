#ifndef OBJMODEL_CLASS_H
#define OBJMODEL_CLASS_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class ObjModel {
public:
    ObjModel(const std::string& filepath);

    const std::vector<GLfloat>& GetVertices() const;
    const std::vector<GLuint>& GetIndices() const;

private:
    std::vector<GLfloat> vertices; // Combined vertex positions, normals, and texture coordinates
    std::vector<GLuint> indices; // Vertex indices

    void LoadObj(const std::string& filepath);
};

#endif