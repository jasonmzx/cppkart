#ifndef RENDERRSRCMANAGER_HPP
#define RENDERRSRCMANAGER_HPP

#include <memory>
#include <unordered_map> // For geometry caching

#include "GLRenderer.hpp"
#include "core/gl/Texture.h"
#include "core/loaders/ObjModel.hpp"

typedef std::unordered_map<std::string, std::shared_ptr<Geometry>> GeometryCache;
typedef std::unordered_map<std::string, std::shared_ptr<Texture>> TextureCache;
typedef std::unordered_map<std::string, std::shared_ptr<ObjModel>> ModelCache;


class RenderRsrcManager {

    public:
        RenderRsrcManager() = default;
        void debugPrint();
        
        //* Caching Geoms, Textures, Models to avoid duplications of identical objects
        GeometryCache geometryCache;
        TextureCache textureCache;
        ModelCache modelCache;

        
        std::shared_ptr<Geometry> getOrCreateGeometry(const std::string& modelIdentifier, 
                        const std::vector<GLfloat>& verts, const std::vector<GLuint>& indices);

        std::shared_ptr<Geometry> tryGetGeometry(const std::string& modelIdentifier);

        std::shared_ptr<Texture> loadTex(const std::string& texId, const std::string& filePath, bool rgbAlpha);

        std::shared_ptr<Texture> tryGetTex(const std::string& texId);

        std::shared_ptr<ObjModel> loadModel(const std::string& modelId, const std::string& filePath);

        std::shared_ptr<ObjModel> tryGetModel(const std::string& modelId);



};

#endif