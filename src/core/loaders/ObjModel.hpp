#ifndef OBJMODEL_CLASS_HPP
#define OBJMODEL_CLASS_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "core/ID.hpp"

class ObjModel : public ID {

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