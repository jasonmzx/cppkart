#include "PhysicsChunkManager.hpp"

//Constructor
PhysicsChunkManager::PhysicsChunkManager(const std::vector<GLfloat>& verts, const std::string& filename) {
    SCALE_FACTOR = 40.0f;
    std::vector<LoadedChunk> chunks = ChunkedMapLoader::loadChunks(filename);

    glm::mat4 chunkModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)); // Placeholder Model Matrix

    for (auto& ldChunk : chunks) {

        auto phyChunk = std::make_unique<StaticTriangleMeshPhysics>(verts, ldChunk.faces, chunkModelMatrix);
        auto newChunk = std::make_unique<PhysicsChunk>(false, std::move(phyChunk));

        newChunk->X_origin = ldChunk.X_origin;
        newChunk->Z_origin = ldChunk.Z_origin;

        chunkVector.push_back(std::move(newChunk)); // Move the unique_ptr to the vector
    }
}

void PhysicsChunkManager::update(btScalar playerX, btScalar playerZ) {
    
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    // Define a radius within which chunks should be active
    constexpr btScalar activationRadius = 55.0; 

    for (auto& chunkUniquePtr : chunkVector) {

        PhysicsChunk& chunk = *chunkUniquePtr; 

        // Calculate distance from player to chunk origin
        btScalar distanceX = playerX - (chunk.X_origin)*SCALE_FACTOR;
        btScalar distanceZ = playerZ - (chunk.Z_origin)*SCALE_FACTOR;
        btScalar distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

        if (distance <= activationRadius && !chunk.active) {

            // Activate chunk and add its rigid body to the physics world
            chunk.active = true;
            physicsWorld->dynamicsWorld->addRigidBody(chunk.rigidMeshChunk.get()->meshRigidBody.get());

        } else if (distance > activationRadius && chunk.active) {

            // Deactivate chunk and remove its rigid body from the physics world
            chunk.active = false;
            physicsWorld->dynamicsWorld->removeRigidBody(chunk.rigidMeshChunk.get()->meshRigidBody.get());
        }
    }
}

PhysicsChunkManager::~PhysicsChunkManager() {
    chunkVector.clear();
}