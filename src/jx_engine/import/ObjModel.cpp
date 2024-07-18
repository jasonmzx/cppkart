#include "ObjModel.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <random>
#include <string>
#include <algorithm> // For std::generate

#include "core/ID.hpp"

ObjModel::ObjModel(const std::string &filepath)
{
    LoadObj(filepath);
    hashID = createHashedID(filepath);
}

const std::vector<GLfloat> &ObjModel::GetVertices(int meshIndex) const
{
    return meshes.at(meshIndex).vertices;
}

const std::vector<GLuint> &ObjModel::GetIndices(int meshIndex) const
{
    return meshes.at(meshIndex).indices;
}

void ObjModel::LoadObj(const std::string &filepath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    Logger *logger = Logger::getInstance();

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        logger->log(Logger::ERROR, "ObjModel.cpp | Failed to load the model: " + filepath);
        return;
    } else {
        logger->log(Logger::INFO, "ObjModel.cpp | Loaded the model: " + filepath);
    }

    for (int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
    {

        aiMesh *mesh = scene->mMeshes[meshIndex];

        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        // Get the material for the mesh
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // Get the diffuse color from the material
        aiColor3D color(1.0f, 1.0f, 1.0f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

        // Extract vertex data
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);

            if (mesh->mTextureCoords[0])
            { // If there are texture coords
                
                
                float u = mesh->mTextureCoords[0][i].x;
                float v = mesh->mTextureCoords[0][i].y;

                vertices.push_back(u);
                vertices.push_back(v);

            }

            if (mesh->mNormals)
            { // If there are normals
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            }
        }

        // Extract index data
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        MeshData meshData;
        meshData.vertices = vertices;
        meshData.indices = indices;

        meshes.push_back(meshData);
    }
    
    logger->log(Logger::INFO, "ObjModel.cpp | Loaded Model: " + filepath);
}