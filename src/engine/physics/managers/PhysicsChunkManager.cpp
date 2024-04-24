#include "PhysicsChunkManager.hpp"

//Constructor
PhysicsChunkManager::PhysicsChunkManager(const std::vector<GLfloat>& verts, const std::string& filename){

    SCALE_FACTOR = 20.0f;

    std::vector<LoadedChunk> chunks = ChunkedMapLoader::loadChunks(filename);

  for(int i = 0; i < chunks.size(); i++) {

    LoadedChunk ldChunk = chunks[i];

    glm::mat4 chunkModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)); //!This is still a placeholder Model Matrix

    StaticTriangleMeshPhysics phyChunk = StaticTriangleMeshPhysics(verts, ldChunk.faces, chunkModelMatrix);

    PhysicsChunk newChunk = PhysicsChunk(false, phyChunk);
    
    newChunk.X_origin = ldChunk.X_origin;
    newChunk.Z_origin = ldChunk.Z_origin;

    chunkVector.push_back(newChunk);

  }

}

// void PhysicsChunkManager::debugMapPrint() {
//     // for (int y = 0; y < N_chunks_y; ++y) {
//     //     for (int x = 0; x < N_chunks_x; ++x) {
//     //         // Calculate the index in the chunkVector based on x and y
//     //         int index = y * N_chunks_x + x;
//     //         if (index < chunkVector.size()) {
//     //             // Access the chunk at the calculated index
//     //             const PhysicsChunk& chunk = chunkVector[index];

//     //             // Print details of the chunk
//     //             printf("(%d, %d | %d ): %c\t", 
//     //                    chunk.X_origin, chunk.Z_origin, chunk.debugSize,
//     //                    chunk.active ? 'T' : 'F');
//     //         }
//     //     }
//     //     printf("\n"); // New line after each row
//     // }

// }

void PhysicsChunkManager::update(btScalar playerX, btScalar playerZ) {
    
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    // Define a radius within which chunks should be active
    constexpr btScalar activationRadius = 55.0; 

    for (PhysicsChunk& chunk : chunkVector) {
        // Calculate distance from player to chunk origin
        btScalar distanceX = playerX - (chunk.X_origin)*SCALE_FACTOR;
        btScalar distanceZ = playerZ - (chunk.Z_origin)*SCALE_FACTOR;
        btScalar distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

        if (distance <= activationRadius && !chunk.active) {

            // Activate chunk and add its rigid body to the physics world
            chunk.active = true;
            physicsWorld->dynamicsWorld->addRigidBody(chunk.rigidMeshChunk.meshRigidBody);

        } else if (distance > activationRadius && chunk.active) {

            // Deactivate chunk and remove its rigid body from the physics world
            chunk.active = false;
            physicsWorld->dynamicsWorld->removeRigidBody(chunk.rigidMeshChunk.meshRigidBody);
        }
    }
}

PhysicsChunkManager::~PhysicsChunkManager() {
    chunkVector.clear();
}