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

std::shared_ptr<Geometry> RenderRsrcManager::tryGetGeometry(const std::string& modelIdentifier) {
    auto iter = geometryCache.find(modelIdentifier);
    
    if (iter != geometryCache.end()) {
        // Found existing geometry, return it
        return iter->second;
    } else {
        // Geometry not found, return nullptr
        return nullptr;
    }
}

//----- tex

std::shared_ptr<Texture> RenderRsrcManager::loadTex(const std::string& texId, const std::string& filePath) {
    auto iter = textureCache.find(texId);
    if (iter != textureCache.end()) {
        // Texture found in cache, return it
        return iter->second;
    } else {
        // Texture not found, load and cache it
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(filePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
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

void RenderRsrcManager::debugPrint() {
    printf("Number of Textures active: %zu \n", textureCache.size());
    printf("Number of Geometries active: %zu \n", geometryCache.size());
}
