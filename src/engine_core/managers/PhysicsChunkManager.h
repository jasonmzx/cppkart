#ifndef PHYSICSCHUNKMANAGER_CLASS_H
#define PHYSICSCHUNKMANAGER_CLASS_H

#include <vector>
#include <btBulletDynamicsCommon.h>

//Local Imports:
#include "terrain_gen/MapChunker.h"
#include "engine_core/bullet_models/TerrainPhysics.h" 
#include "../singletons/PhysicsWorldSingleton.h"


// NOTE: TerrainPhysics doesn't have a default empty constructor, so this struct needs to be isntanciated immediately
struct PhysicsChunk {
    bool active;
    TerrainPhysics heightmapChunk;

    int X_origin; int Z_origin;

    PhysicsChunk(bool isActive, const TerrainPhysics& terrain)
    : active(isActive), heightmapChunk(terrain) {}
};

class PhysicsChunkManager {

public:
    PhysicsChunkManager(const std::string& filename);

    void update(btScalar playerX, btScalar playerZ);

    void debugMapPrint();

    int chunk_size = 25; //Represents Width & Length of chunk (X & Z)

private:
    std::vector<PhysicsChunk> chunkVector;
    btScalar globalChunkMin; 
    btScalar globalChunkMax;
    int N_chunks_x;
    int N_chunks_y;
};

#endif