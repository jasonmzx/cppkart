#include "PhysicsChunkManager.hpp"

#include "jx_engine/core/ThreadPool.hpp"
#include <chrono>
#include <filesystem>
#include <future>

Logger *logger = Logger::getInstance();

// Constructor
//  PhysicsChunkManager::PhysicsChunkManager(const std::string& filename, float scaleFac) {
//      SCALE_FACTOR = scaleFac;

//     // Get tiem now

//     auto chunk_loading_start = std::chrono::high_resolution_clock::now();

//     std::vector<LoadedChunk> chunks = PhysChunkedMapLoader::loadChunks(filename);

//     auto chunk_loading_end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> chunk_loading_time = chunk_loading_end - chunk_loading_start;
//     logger->log(Logger::INFO, "[PHYSICS CHUNKED MAP LOADER] : Chunk Parsing Time: " + std::to_string(chunk_loading_time.count()) + "s");

//     glm::mat4 chunkModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)); // Placeholder Model Matrix

//     for (auto& ldChunk : chunks) {

//         auto phyChunk = std::make_unique<StaticTriangleMeshPhysics>(ldChunk.triangle_ordered_verts, chunkModelMatrix, SCALE_FACTOR);
//         auto newChunk = std::make_unique<PhysicsChunk>(false, std::move(phyChunk));

//         newChunk->X_origin = ldChunk.X_origin;
//         newChunk->Z_origin = ldChunk.Z_origin;

//         chunkVector.push_back(std::move(newChunk)); // Move the unique_ptr to the vector
//     }

//     auto chunk_loading_end2 = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> chunk_loading_time2 = chunk_loading_end2 - chunk_loading_start;

//     logger->log(Logger::INFO, "[PHYSICS CHUNKED MAP LOADER] : Total Chunk Loading Time: " + std::to_string(chunk_loading_time2.count()) + "s");

// }

PhysicsChunkManager::PhysicsChunkManager(const std::string &directory, float scaleFac, size_t num_threads)
{
    SCALE_FACTOR = scaleFac;

    auto chunk_loading_start = std::chrono::high_resolution_clock::now();

    // Create a ThreadPool with num_threads worker threads for loading chunks
    ThreadPool loadPool(num_threads);

    // Vector to hold futures representing the load tasks
    std::vector<std::future<std::vector<LoadedChunk>>> loadFutures;

    // Iterate over files in the given directory
    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.path().extension() == ".txt")
        {
            // Enqueue a task to load chunks from this file
            loadFutures.emplace_back(
                loadPool.enqueue([filename = entry.path().string()]
                                 { return PhysChunkedMapLoader::loadChunks(filename); }));
        }
    }

    // Create a ThreadPool for processing loaded chunks
    ThreadPool processPool(num_threads);

    // Vector to hold futures representing the process tasks
    std::vector<std::future<void>> processFutures;

    // Placeholder model matrix
    glm::mat4 chunkModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    // Process the results of each future
    for (auto &loadFuture : loadFutures)
    {
        // future.get() will block until the load task is complete
        std::vector<LoadedChunk> chunks = loadFuture.get();

        // Enqueue a task to process each chunk
        for (auto &ldChunk : chunks)
        {
            // clang-format off
            processFutures.emplace_back(
                processPool.enqueue([this, ldChunk, chunkModelMatrix]()
                    {
                        auto phyChunk = std::make_unique<StaticTriangleMeshPhysics>(ldChunk.triangle_ordered_verts, chunkModelMatrix, SCALE_FACTOR);
                        auto newChunk = std::make_unique<PhysicsChunk>(false, std::move(phyChunk));

                        newChunk->X_origin = ldChunk.X_origin;
                        newChunk->Z_origin = ldChunk.Z_origin;

                        std::lock_guard<std::mutex> lock(this->chunkMutex);
                        this->chunkVector.push_back(std::move(newChunk)); // Move the unique_ptr to the vector
                    }
                )
            );
            // clang-format on
        }
    }

    // Wait for all process tasks to complete
    for (auto &processFuture : processFutures)
    {
        processFuture.get();
    }

    auto chunk_loading_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> chunk_loading_time = chunk_loading_end - chunk_loading_start;

    logger->log(Logger::TIMER, "[PHYSICS CHUNKED MAP LOADER] : Total Chunk Loading Time: " + std::to_string(chunk_loading_time.count()) + "s");
}

void PhysicsChunkManager::update(btScalar playerX, btScalar playerZ)
{

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    static int rigidbodychanges = 0;
    // Define a radius within which chunks should be active
    constexpr btScalar activationRadius = 200.0;

    if (physicsWorld == nullptr)
    {
        logger->log(Logger::ERROR, "PhysicsWorldSingleton instance is null.");
        return;
    }

    if (physicsWorld->dynamicsWorld == nullptr)
    {
        logger->log(Logger::ERROR, "Physics dynamicsWorld is null.");
        return;
    }

    for (auto &chunkUniquePtr : chunkVector)
    {

        if (!chunkUniquePtr)
        {
            logger->log(Logger::ERROR, "Chunk unique_ptr is null.");
            continue;
        }

        PhysicsChunk &chunk = *chunkUniquePtr;

        if (!chunk.rigidMeshChunk)
        {
            logger->log(Logger::ERROR, "Chunk rigidMeshChunk is null.");
            continue;
        }

        if (!chunk.rigidMeshChunk->meshRigidBody)
        {
            logger->log(Logger::ERROR, "Chunk meshRigidBody is null.");
            continue;
        }

        btRigidBody *body = chunk.rigidMeshChunk->meshRigidBody;
        body->setUserPointer((void *)2);
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

        if (body == nullptr)
        {
            logger->log(Logger::ERROR, "meshRigidBody is null.");
            continue;
        }

        // Calculate distance from player to chunk origin
        btScalar distanceX = playerX - (chunk.X_origin) * SCALE_FACTOR;
        btScalar distanceZ = playerZ - (chunk.Z_origin) * SCALE_FACTOR;
        btScalar distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

        if (distance <= activationRadius && !chunk.active)
        {

            rigidbodychanges++;
            logger->log(Logger::WARN, "AD_B: " + std::to_string(rigidbodychanges) + " | Chunk activating at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin) + " Distance: " + std::to_string(distance) + " Activation Radius: " + std::to_string(activationRadius));

            // Activate chunk and add its rigid body to the physics world
            chunk.active = true;
            logger->log(Logger::INFO, "Attempting to addRigidBody");
            physicsWorld->dynamicsWorld->addRigidBody(body, COLLISION_GROUP_ALL, COLLISION_GROUP_ALL);
        }
        else if (distance > activationRadius && chunk.active)
        {

            rigidbodychanges++;
            logger->log(Logger::WARN, "RM_B: " + std::to_string(rigidbodychanges) + " | Chunk deactivating at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin) + " Distance: " + std::to_string(distance) + " Activation Radius: " + std::to_string(activationRadius));

            // Deactivate chunk and remove its rigid body from the physics world
            chunk.active = false;
            logger->log(Logger::INFO, "Attempting to removeRigidBody");
            physicsWorld->dynamicsWorld->removeRigidBody(body);
        }
    }
}

//! DEPRECATED FUNCTION : (Maybe use for testing)

void PhysicsChunkManager::ActiveAll()
{
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
    Logger *logger = Logger::getInstance();

    for (auto &chunkUniquePtr : chunkVector)
    {
        PhysicsChunk &chunk = *chunkUniquePtr;
        if (!chunk.active)
        {

            logger->log(Logger::WARN, "AA-DEBUG: Chunk activating at X: " + std::to_string(chunk.X_origin) + " Z: " + std::to_string(chunk.Z_origin));

            chunk.active = true;
            physicsWorld->dynamicsWorld->addRigidBody(chunk.rigidMeshChunk->meshRigidBody);
        }
    }
}

PhysicsChunkManager::~PhysicsChunkManager()
{
    chunkVector.clear();
}