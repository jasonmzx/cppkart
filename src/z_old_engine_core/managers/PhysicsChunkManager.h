#ifndef PHYSICSCHUNKMANAGER_CLASS_H
#define PHYSICSCHUNKMANAGER_CLASS_H

#include <vector>
#include <btBulletDynamicsCommon.h>

// Local Imports:
#include "terrain_gen/MapChunker.h"
#include "engine_core/bullet_models/TerrainPhysics.h"
#include "engine_core/singletons/PhysicsWorldSingleton.h"

// NOTE: TerrainPhysics doesn't have a default empty constructor, so this struct needs to be isntanciated immediately
struct PhysicsChunk
{
    bool active;
    TerrainPhysics heightmapChunk;

    int X_origin;
    int Z_origin;
    int debugSize;

    PhysicsChunk(bool isActive, const TerrainPhysics &terrain)
        : active(isActive), heightmapChunk(terrain) {}
};

class PhysicsChunkManager
{

public:
    PhysicsChunkManager(const std::string &filename);
    ~PhysicsChunkManager();

    void update(btScalar playerX, btScalar playerZ);

    void debugMapPrint();

    int chunk_size = 10; // Represents Width & Length of chunk (X & Z)
    btScalar globalChunkMin;
    btScalar globalChunkMax;

private:
    std::vector<PhysicsChunk> chunkVector;
    int N_chunks_x;
    int N_chunks_y;

    float SCALE_FACTOR;
};

#endif