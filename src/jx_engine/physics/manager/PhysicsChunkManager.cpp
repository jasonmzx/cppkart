#include "PhysicsChunkManager.hpp"

//Constructor
PhysicsChunkManager::PhysicsChunkManager(const std::string& filename, float scaleFac) {
    SCALE_FACTOR = scaleFac;
    std::vector<LoadedChunk> chunks = PhysChunkedMapLoader::loadChunks(filename);

    glm::mat4 chunkModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)); // Placeholder Model Matrix

    for (auto& ldChunk : chunks) {

        auto phyChunk = std::make_unique<StaticTriangleMeshPhysics>(ldChunk.triangle_ordered_verts, chunkModelMatrix, SCALE_FACTOR);
        auto newChunk = std::make_unique<PhysicsChunk>(false, std::move(phyChunk));

        newChunk->X_origin = ldChunk.X_origin;
        newChunk->Z_origin = ldChunk.Z_origin;

        chunkVector.push_back(std::move(newChunk)); // Move the unique_ptr to the vector
    }
}

void PhysicsChunkManager::update(btScalar playerX, btScalar playerZ) {
    
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
    Logger* logger = Logger::getInstance();

    static int rigidbodychanges = 0;
    // Define a radius within which chunks should be active
    constexpr btScalar activationRadius = 65.0; 

    if (physicsWorld == nullptr) {
        logger->log(Logger::ERROR, "PhysicsWorldSingleton instance is null.");
        return;
    }

    if (physicsWorld->dynamicsWorld == nullptr) {
        logger->log(Logger::ERROR, "Physics dynamicsWorld is null.");
        return;
    }

    for (auto& chunkUniquePtr : chunkVector) {
        
        if (!chunkUniquePtr) {
            logger->log(Logger::ERROR, "Chunk unique_ptr is null.");
            continue;
        }

        PhysicsChunk& chunk = *chunkUniquePtr; 

        if (!chunk.rigidMeshChunk) {
            logger->log(Logger::ERROR, "Chunk rigidMeshChunk is null.");
            continue;
        }

        if (!chunk.rigidMeshChunk->meshRigidBody) {
            logger->log(Logger::ERROR, "Chunk meshRigidBody is null.");
            continue;
        }

        btRigidBody* body = chunk.rigidMeshChunk->meshRigidBody;

        if (body == nullptr) {
            logger->log(Logger::ERROR, "meshRigidBody is null.");
            continue;
        }


        // Calculate distance from player to chunk origin
        btScalar distanceX = playerX - (chunk.X_origin) * SCALE_FACTOR;
        btScalar distanceZ = playerZ - (chunk.Z_origin) * SCALE_FACTOR;
        btScalar distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

        if (distance <= activationRadius && !chunk.active) {

            rigidbodychanges++;
            logger->log(Logger::WARN, "AD_B: " + std::to_string(rigidbodychanges) + " | Chunk activating at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin) + " Distance: " + std::to_string(distance) + " Activation Radius: " + std::to_string(activationRadius));

            // Activate chunk and add its rigid body to the physics world
            chunk.active = true;
            logger->log(Logger::INFO, "Attempting to addRigidBody");
            physicsWorld->dynamicsWorld->addRigidBody(body);

        } else if (distance > activationRadius && chunk.active) {
            
            rigidbodychanges++;
            logger->log(Logger::WARN, "RM_B: " + std::to_string(rigidbodychanges) + " | Chunk deactivating at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin) + " Distance: " + std::to_string(distance) + " Activation Radius: " + std::to_string(activationRadius));

            // Deactivate chunk and remove its rigid body from the physics world
            chunk.active = false;
            logger->log(Logger::INFO, "Attempting to removeRigidBody");
            physicsWorld->dynamicsWorld->removeRigidBody(body);
        }
    }
}


void PhysicsChunkManager::ActiveAll() {
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
    Logger* logger = Logger::getInstance();

    for (auto& chunkUniquePtr : chunkVector) {
        PhysicsChunk& chunk = *chunkUniquePtr;
        if (!chunk.active) {

            logger->log(Logger::WARN, "AA-DEBUG: Chunk activating at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin));

            chunk.active = true;
            physicsWorld->dynamicsWorld->addRigidBody(chunk.rigidMeshChunk.get()->meshRigidBody);
        }
    }
}

PhysicsChunkManager::~PhysicsChunkManager() {
    chunkVector.clear();
}