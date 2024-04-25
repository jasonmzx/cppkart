#ifndef PHYSICSCHUNKMANAGER_CLASS_HPP
#define PHYSICSCHUNKMANAGER_CLASS_HPP

#include <vector>
#include <bullet/btBulletDynamicsCommon.h>

// Local Imports:
#include "engine/physics/PhysicsWorldSingleton.h"
#include "engine/physics/StaticTriangleMeshPhysics.hpp"
#include "core/loaders/ChunkedMapLoader.hpp"

// NOTE: TerrainPhysics doesn't have a default empty constructor, so this struct needs to be isntanciated immediately
struct PhysicsChunk
{
    StaticTriangleMeshPhysics rigidMeshChunk;
    bool active;

    float X_origin;
    float Z_origin;

    PhysicsChunk(bool isActive, const StaticTriangleMeshPhysics &mesh)
        : active(isActive), rigidMeshChunk(mesh) {}
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
    std::vector<PhysicsChunk> chunkVector;

    float SCALE_FACTOR;
};

#endif