#ifndef OBJMODEL_CLASS_HPP
#define OBJMODEL_CLASS_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "core/ID.hpp"

#include "jx_engine/logs/Logger.hpp"

class ObjModel : public ID {

struct MeshData {
    std::vector<GLfloat> vertices; // Combined vertex positions, normals, and texture coordinates
    std::vector<GLuint> indices; // Vertex indices
};

public:
    ObjModel(const std::string& filepath);

    const std::vector<GLfloat>& GetVertices(int meshIndex) const;
    const std::vector<GLuint>& GetIndices(int meshIndex) const;

private:
    std::vector<MeshData> meshes;

    void LoadObj(const std::string& filepath);
};

#endif