#ifndef PHYSICSCHUNKMANAGER_CLASS_HPP
#define PHYSICSCHUNKMANAGER_CLASS_HPP

#include <vector>
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>

// Local Imports:
#include "engine/physics/PhysicsWorldSingleton.h"
#include "engine/physics/StaticTriangleMeshPhysics.hpp"
#include "core/loaders/ChunkedMapLoader.hpp"

struct PhysicsChunk
{
    std::unique_ptr<StaticTriangleMeshPhysics> rigidMeshChunk;
    bool active;
    float X_origin;
    float Z_origin;

    PhysicsChunk(bool isActive, std::unique_ptr<StaticTriangleMeshPhysics> mesh)
        : active(isActive), rigidMeshChunk(std::move(mesh)) {}
};

class PhysicsChunkManager
{

public:
    PhysicsChunkManager(const std::vector<GLfloat>& verts, const std::string& filename);
    ~PhysicsChunkManager();

    void update(btScalar playerX, btScalar playerZ);

    //void debugMapPrint();

    // btScalar globalChunkMin;
    // btScalar globalChunkMax;

private:
    std::vector<std::unique_ptr<PhysicsChunk>> chunkVector;

    float SCALE_FACTOR;
};

#endif