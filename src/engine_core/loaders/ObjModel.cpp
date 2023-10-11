#include "ObjModel.h"
#include <fstream>
#include <sstream>

ObjModel::ObjModel(const std::string& filepath) {
    LoadObj(filepath);
}

const std::vector<GLfloat>& ObjModel::GetVertices() const {
    return vertices;
}

const std::vector<GLuint>& ObjModel::GetIndices() const {
    return indices;
}

void ObjModel::LoadObj(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        // Handle error
        return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<glm::vec3> temp_normals;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);

        } else if (prefix == "vt") {
            glm::vec2 tex;
            iss >> tex.x >> tex.y;
            temp_texcoords.push_back(tex);

        } else if (prefix == "vn") {
            glm::vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);

        } else if (prefix == "f") {
            GLuint i, j, k;
            char slash;
            for (int n = 0; n < 3; ++n) {
                iss >> i >> slash >> j >> slash >> k;
                // Adjust for 0-based indexing
                i--; j--; k--;

                vertices.push_back(temp_positions[i].x);
                vertices.push_back(temp_positions[i].y);
                vertices.push_back(temp_positions[i].z);

                vertices.push_back(temp_normals[k].x);
                vertices.push_back(temp_normals[k].y);
                vertices.push_back(temp_normals[k].z);

                vertices.push_back(temp_texcoords[j].x);
                vertices.push_back(temp_texcoords[j].y);

                indices.push_back((GLuint)indices.size());
            }
        }
    }
    file.close();
}