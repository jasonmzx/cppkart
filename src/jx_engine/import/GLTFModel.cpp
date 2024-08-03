#include "GLTFModel.hpp"

GLTFModel::GLTFModel(const std::string &filepath)
{
    Logger* logger = Logger::getInstance();
    logger->log(Logger::INFO, "GLTFModel.cpp | Loading the model: " + filepath);

    loadGLTF(filepath);

    for(auto& image : asset.images) {
        std::cout << "Image: " << image.name << '\n';
    }   

    for(auto& material : asset.materials) {
        std::cout << "Material: " << material.name << '\n';
    }

    for(auto& mesh : asset.meshes) {
        std::cout << "Mesh: " << mesh.name << '\n';
    }

}

bool GLTFModel::loadGLTF(std::filesystem::path filepath)
{
    Logger* logger = Logger::getInstance();

    if (!std::filesystem::exists(filepath)) {
        logger->log(Logger::ERROR, "GLTFModel.cpp | File does not exist: " + filepath.string());
        return false;
    }

    static constexpr auto supportedExtensions =
        fastgltf::Extensions::KHR_mesh_quantization |
        fastgltf::Extensions::KHR_texture_transform |
        fastgltf::Extensions::KHR_materials_variants;

    constexpr auto gltfOptions =
        fastgltf::Options::DontRequireValidAssetMember |
        fastgltf::Options::AllowDouble |
        fastgltf::Options::LoadExternalBuffers |
        fastgltf::Options::LoadExternalImages |
        fastgltf::Options::GenerateMeshIndices;

    fastgltf::Parser parser(supportedExtensions);

    auto gltfFile = fastgltf::MappedGltfFile::FromPath(filepath);
    if (!gltfFile) {
        std::cerr << "Failed to open glTF file: " << fastgltf::getErrorMessage(gltfFile.error()) << '\n';
        return false;
    }

    auto asset = parser.loadGltf(gltfFile.get(), filepath.parent_path(), gltfOptions);
    if (asset.error() != fastgltf::Error::None) {
        std::cerr << "Failed to load glTF: " << fastgltf::getErrorMessage(asset.error()) << '\n';
        return false;
    }

    this->asset = std::move(asset.get());

    return true;
}


