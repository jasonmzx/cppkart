#include "RenderRsrcManager.hpp"

std::shared_ptr<Geometry> RenderRsrcManager::getOrCreateGeometry(const std::string& modelIdentifier, 
const std::vector<GLfloat>& verts, const std::vector<GLuint>& indices) {

    auto iter = geometryCache.find(modelIdentifier);
    if (iter != geometryCache.end()) {
        // Found existing geometry, reuse it
        return iter->second;
    } else {
        // Create new Geometry object
        auto geometry = std::make_shared<Geometry>(verts, indices);
        geometryCache[modelIdentifier] = geometry;
        return geometry;
    }
}

std::shared_ptr<Geometry> RenderRsrcManager::tryGetGeometry(const std::string& modelIdentifier, int meshIndex) {
    
    std::string geomId = modelIdentifier + std::to_string(meshIndex);
    
    auto iter = geometryCache.find(geomId);
    
    if (iter != geometryCache.end()) {
        // Found existing geometry, return it
        return iter->second;
    } else {
        // Geometry not found, So Load Model and Create Geometry
        
        auto model = tryGetModel(modelIdentifier);
        
        if(model == nullptr) {
            model = loadModel(modelIdentifier, modelIdentifier);
        }

        //printf("New Model Loaded!");
          std::vector<GLfloat> verts = model->GetVertices(meshIndex);
          std::vector<GLuint> indices = model->GetIndices(meshIndex);
          return getOrCreateGeometry(geomId, verts, indices);  
    }
}

std::shared_ptr<Geometry> RenderRsrcManager::tryGetCombinedGeometry(
    const std::string& modelIdentifier, const std::vector<int>& meshIndices) 
{
    
    std::string geomId = modelIdentifier;

    for (int i = 0; i < meshIndices.size(); i++) {
        geomId += std::to_string(meshIndices[i]);
    }

    auto iter = geometryCache.find(geomId);

    if (iter != geometryCache.end()) {
        // Found existing geometry, return it
        return iter->second;
    } else {
        // Geometry not found, so load model and create geometry

        auto model = tryGetModel(modelIdentifier);

        if (model == nullptr) {
            model = loadModel(modelIdentifier, modelIdentifier);
        }

        // Get combined vertices and indices
        std::vector<GLfloat> verts;
        std::vector<GLuint> indices;
        GLuint vertexOffset = 0;

        printf("MeshIndices Size: %zu\n", meshIndices.size());

        for (int i = 0; i < meshIndices.size(); i++) {
            int meshIndex = meshIndices.at(i);

            std::vector<GLfloat> meshVerts = model->GetVertices(meshIndex);
            std::vector<GLuint> meshIndices = model->GetIndices(meshIndex);

            // Adjust indices with the current vertex offset
            for (auto& index : meshIndices) {
                index += vertexOffset;
            }

            // Insert vertices and adjusted indices into the combined lists
            verts.insert(verts.end(), meshVerts.begin(), meshVerts.end());
            indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());

            // Update vertexOffset for the next mesh
            vertexOffset += static_cast<GLuint>(meshVerts.size() / 11); // Assuming 3 floats per vertex (x, y, z)
        }

        return getOrCreateGeometry(geomId, verts, indices);
    }
}

//----- tex

std::shared_ptr<Texture> RenderRsrcManager::loadTex(const std::string& texId, const std::string& filePath, bool rgbAlpha) {
    auto iter = textureCache.find(texId);
    if (iter != textureCache.end()) {
        // Texture found in cache, return it
        return iter->second;
    } else {
        // Texture not found, load and cache it
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(filePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE, rgbAlpha);
        textureCache[texId] = texture;
        return texture;
    }
}

std::shared_ptr<Texture> RenderRsrcManager::tryGetTex(const std::string& texId) {
    auto iter = textureCache.find(texId);
    
    if (iter != textureCache.end()) {
        // Found existing geometry, return it
        return iter->second;
    } else {
        // Geometry not found, return nullptr
        return nullptr;
    }
}

// ----- model

std::shared_ptr<ObjModel> RenderRsrcManager::loadModel(const std::string& modelId, const std::string& filePath) {
    auto iter = modelCache.find(modelId);
    if (iter != modelCache.end()) {
        // Model found in cache, return it
        return iter->second;
    } else {
        // Model not found, load and cache it
        std::shared_ptr<ObjModel> mdl = std::make_shared<ObjModel>(filePath);
        modelCache[modelId] = mdl;
        return mdl;
    }
}

std::shared_ptr<ObjModel> RenderRsrcManager::tryGetModel(const std::string& modelId) {
    auto iter = modelCache.find(modelId);
    
    if (iter != modelCache.end()) {
        // Found existing model, return it
        return iter->second;
    } else {
        // Model not found, return nullptr
        return nullptr;
    }
}

void RenderRsrcManager::debugPrint() {
    printf("RSRC MGMT [ : TEX %zu , GEOM %zu , MODEL %zu ] \n", textureCache.size(), geometryCache.size(), modelCache.size());

}

// Singleton stuff

RenderRsrcManager* RenderRsrcManager::instance = nullptr;

RenderRsrcManager* RenderRsrcManager::getInstance() {
    if (!instance)
        instance = new RenderRsrcManager();
    return instance;
}
