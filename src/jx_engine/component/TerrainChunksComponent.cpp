#include "TerrainChunksComponent.hpp"

TerrainChunksComponent::TerrainChunksComponent(const std::string& filename, float scaleFactor) {
    physicsChunkManager = std::make_shared<PhysicsChunkManager>(filename, scaleFactor, 16);
    //physicsChunkManager->ActiveAll(); //DEBUG
}

void TerrainChunksComponent::handleUpdateChunksEvent(const Event& event) {
    try {
        auto inputs = std::any_cast<std::pair<btScalar, btScalar>>(event.data);
        btScalar playerX = inputs.first;
        btScalar playerZ = inputs.second;
        
        if (++ticker_counter == 5) {
            physicsChunkManager->update(playerX, playerZ);
            ticker_counter = 0;
        }

    } catch (const std::bad_any_cast& e) {
        Logger::getInstance()->log(Logger::ERROR, "[TerrainChunksComponent] Failed to cast event data: " + std::string(e.what()));
    }

}

void TerrainChunksComponent::tick()
{
    // Do nothing
}