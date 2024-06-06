#include "TerrainChunksComponent.hpp"

TerrainChunksComponent::TerrainChunksComponent(const std::string& filename) {
    physicsChunkManager = std::make_shared<PhysicsChunkManager>(filename);
}

void TerrainChunksComponent::updateChunks(btScalar playerX, btScalar playerZ) {
        physicsChunkManager->update(playerX, playerZ);
}