#ifndef GLTFMODEL_CLASS_HPP
#define GLTFMODEL_CLASS_HPP

#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>
#include <fastgltf/tools.hpp>

#include "jx_engine/logs/Logger.hpp"

class GLTFModel {

public:
    GLTFModel(const std::string& filepath);

    bool loadGLTF(std::filesystem::path filepath);
private:

    fastgltf::Asset asset;
};

#endif