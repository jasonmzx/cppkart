#include "TerrainChunksComponent.hpp"

TerrainChunksComponent::TerrainChunksComponent(const std::string& filename, float scaleFactor) {
    physicsChunkManager = std::make_shared<PhysicsChunkManager>(filename, scaleFactor);
    //physicsChunkManager->ActiveAll(); //DEBUG
}

void TerrainChunksComponent::updateChunks(btScalar playerX, btScalar playerZ) {
    ticker_counter++;
    if (ticker_counter == 5) {
        physicsChunkManager->update(playerX, playerZ);
        ticker_counter = 0;
    }
}