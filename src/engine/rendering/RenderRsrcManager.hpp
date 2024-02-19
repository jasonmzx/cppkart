#ifndef RENDERRSRCMANAGER_HPP
#define RENDERRSRCMANAGER_HPP

#include <memory>
#include <unordered_map> // For geometry caching

#include "GLRenderer.hpp"
#include "core/gl/Texture.h"

typedef std::unordered_map<std::string, std::shared_ptr<Geometry>> GeometryCache;
typedef std::unordered_map<std::string, std::shared_ptr<Texture>> TextureCache;

class RenderRsrcManager {

    public:
        RenderRsrcManager() = default;
        void debugPrint();
        
        //Caching Geoms. to avoid duplications of identical geometries
        GeometryCache geometryCache;
        TextureCache textureCache;

        // Function that checks Geometry Cache, and handles creation of geometry or lookup
        
        std::shared_ptr<Geometry> getOrCreateGeometry(const std::string& modelIdentifier, 
                        const std::vector<GLfloat>& verts, const std::vector<GLuint>& indices);

        std::shared_ptr<Geometry> tryGetGeometry(const std::string& modelIdentifier);


        std::shared_ptr<Texture> loadTex(const std::string& texId, const std::string& filePath);

        std::shared_ptr<Texture> tryGetTex(const std::string& texId);

};

#endif