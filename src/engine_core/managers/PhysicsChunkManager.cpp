#include "PhysicsChunkManager.h"

//Constructor
PhysicsChunkManager::PhysicsChunkManager(const std::string& filename){

    std::vector<std::vector<float>> tempChunkVec;

    bool chunkHeightMap = chunkHeightDataFromIMG(filename.c_str(), &chunk_size, tempChunkVec, N_chunks_x, N_chunks_y, globalChunkMin, globalChunkMax);

    printf("Nx: %d  , Ny: %d", N_chunks_x, N_chunks_y);

    // Centre Coords

    int cX = N_chunks_x/2;
    int cZ = N_chunks_y/2;

    //* Building Chunk Vector:
    for (size_t i = 0; i < tempChunkVec.size(); ++i) {
        const auto& heightDataVec = tempChunkVec[i];
        
        float* heightData = new float[heightDataVec.size()];
        std::copy(heightDataVec.begin(), heightDataVec.end(), heightData);

        //TODO: Unsafe if N_chunks x or y is 0, assert for that

        int rX = i % N_chunks_x; //X Chunk in row
        int rZ = i / N_chunks_y; //Y Chunk (col)
        

        int X_origin = (cX - rX) * chunk_size;
        int Z_origin = (cZ - rZ) * chunk_size;

        //===== EXPERIMENTAL, DOESNT' FULLY WORK ====== 
        
        int offsetFactorX = (std::abs(X_origin) / chunk_size);
        int offsetFactorZ = (std::abs(Z_origin) / chunk_size);


        // if(X_origin < 0){
        //     X_origin += offsetFactorX;
        // } else if (X_origin > 0) {
        //     X_origin -= offsetFactorX;
        // }

        // if(Z_origin < 0){
        //     Z_origin += offsetFactorZ;
        // } else if (Z_origin > 0) {
        //     Z_origin -= offsetFactorZ;
        // }
        //===== EXPERIMENTAL, DOESNT' FULLY WORK ====== 
        
        int X_a = 1;
        int Z_a = 1;

        if(rX == (N_chunks_x - 1)) {
            X_a = 0; X_origin++; //TODO: should be += 0.5, but it's an int, so like change the origins to float
        }

        if(rZ == (N_chunks_y - 1)) {
            Z_a = 0; Z_origin++; //TODO: should be += 0.5, but it's an int, so like change the origins to float
        }

        //! ALWAYS MULTIPLY X and Z Origins by -1 here, or we're FUCKED
        TerrainPhysics terrainChunk(chunk_size+X_a, chunk_size+Z_a, heightData, globalChunkMin, globalChunkMax, -1*X_origin, -1*Z_origin);

        PhysicsChunk chunk(false, terrainChunk);
        chunk.X_origin = X_origin;
        chunk.Z_origin = Z_origin;
        chunk.debugSize = heightDataVec.size();

        chunkVector.push_back(chunk);
    }

    debugMapPrint();

}

void PhysicsChunkManager::debugMapPrint() {
    printf("\n"); // New line after each row
    for (int y = 0; y < N_chunks_y; ++y) {
        for (int x = 0; x < N_chunks_x; ++x) {
            // Calculate the index in the chunkVector based on x and y
            int index = y * N_chunks_x + x;
            if (index < chunkVector.size()) {
                // Access the chunk at the calculated index
                const PhysicsChunk& chunk = chunkVector[index];

                // Print details of the chunk
                printf("(%d, %d | %d ): %c\t", 
                       chunk.X_origin, chunk.Z_origin, chunk.debugSize,
                       chunk.active ? 'T' : 'F');
            }
        }
        printf("\n"); // New line after each row
    }
}

//EXPERIMENTAL

void PhysicsChunkManager::update(btScalar playerX, btScalar playerZ) {
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    // Define a radius within which chunks should be active
    const btScalar activationRadius = 120.0; // Example radius value

    for (PhysicsChunk& chunk : chunkVector) {
        // Calculate distance from player to chunk origin
        btScalar distanceX = playerX - (chunk.X_origin*-1);
        btScalar distanceZ = playerZ - (chunk.Z_origin*-1);
        btScalar distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

        if (distance <= activationRadius && !chunk.active) {
            // Activate chunk and add its rigid body to the physics world
            chunk.active = true;
            physicsWorld->dynamicsWorld->addRigidBody(chunk.heightmapChunk.terrainRigidBody);
        } else if (distance > activationRadius && chunk.active) {
            // Deactivate chunk and remove its rigid body from the physics world
            chunk.active = false;
            physicsWorld->dynamicsWorld->removeRigidBody(chunk.heightmapChunk.terrainRigidBody);
        }
    }
}