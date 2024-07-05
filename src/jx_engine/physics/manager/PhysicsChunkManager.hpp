#ifndef PHYSICSCHUNKMANAGER_CLASS_HPP
#define PHYSICSCHUNKMANAGER_CLASS_HPP

#include <vector>
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>
#include <mutex>

// Local Imports:
#include "jx_engine/physics/PhysicsWorldSingleton.h"
#include "jx_engine/physics/StaticTriangleMeshPhysics.hpp"

#include "jx_engine/import/PhysChunkedMapLoader.hpp"

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
    PhysicsChunkManager(const std::string& filename, float scaleFac, size_t num_threads);
    ~PhysicsChunkManager();

    void update(btScalar playerX, btScalar playerZ);

    void ActiveAll();

    //void debugMapPrint();

    // btScalar globalChunkMin;
    // btScalar globalChunkMax;

    std::mutex chunkMutex;

private:
    std::vector<std::unique_ptr<PhysicsChunk>> chunkVector;

    float SCALE_FACTOR;
};

#endif