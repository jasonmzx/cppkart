#include "PhysicsChunkManager.hpp"

//Constructor
PhysicsChunkManager::PhysicsChunkManager(const std::string& filename) {
    SCALE_FACTOR = 5.0f;
    std::vector<LoadedChunk> chunks = PhysChunkedMapLoader::loadChunks(filename);

    glm::mat4 chunkModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)); // Placeholder Model Matrix

    for (auto& ldChunk : chunks) {

        auto phyChunk = std::make_unique<StaticTriangleMeshPhysics>(ldChunk.triangle_ordered_verts, chunkModelMatrix);
        auto newChunk = std::make_unique<PhysicsChunk>(false, std::move(phyChunk));

        newChunk->X_origin = ldChunk.X_origin;
        newChunk->Z_origin = ldChunk.Z_origin;

        chunkVector.push_back(std::move(newChunk)); // Move the unique_ptr to the vector
    }
}

void PhysicsChunkManager::update(btScalar playerX, btScalar playerZ) {
    
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
    Logger* logger = Logger::getInstance();


    // Define a radius within which chunks should be active
    constexpr btScalar activationRadius = 200.0; 

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
            logger->log(Logger::WARN, "Chunk activated at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin) + " Distance: " + std::to_string(distance) + " Activation Radius: " + std::to_string(activationRadius));

        } else if (distance > activationRadius && chunk.active) {

            // Deactivate chunk and remove its rigid body from the physics world
            chunk.active = false;
            physicsWorld->dynamicsWorld->removeRigidBody(chunk.rigidMeshChunk.get()->meshRigidBody.get());
            logger->log(Logger::WARN, "Chunk deactivated at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin) + " Distance: " + std::to_string(distance) + " Activation Radius: " + std::to_string(activationRadius));
        }
    }
}

void PhysicsChunkManager::ActiveAll() {
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    for (auto& chunkUniquePtr : chunkVector) {
        PhysicsChunk& chunk = *chunkUniquePtr;
        if (!chunk.active) {
            chunk.active = true;
            physicsWorld->dynamicsWorld->addRigidBody(chunk.rigidMeshChunk.get()->meshRigidBody.get());
        }
    }
}

PhysicsChunkManager::~PhysicsChunkManager() {
    chunkVector.clear();
}