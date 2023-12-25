#ifndef PHYSICSCHUNKMANAGER_CLASS_H
#define PHYSICSCHUNKMANAGER_CLASS_H

#include <vector>
#include <btBulletDynamicsCommon.h>

struct PhysicsChunk {
    bool active;
    float* rawHeightData;
};


class PhysicsChunkManager {

public:
    PhysicsChunkManager(const std::string& filename);

    int chunk_size = 50; //Represents Width & Length (X & Z)

private:
    std::vector<PhysicsChunk> chunkVector;
    btScalar globalChunkMin; 
    btScalar globalChunkMax;
    int N_chunks_x;
    int N_chunks_y;
};

#endif