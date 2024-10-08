#include "MapChunkSystem.hpp"

MapChunkComponent MapChunkSystem::buildMapChunkComponent(const std::string& filename, float scaleFactor) {
    MapChunkComponent mcc;
    mcc.physicsChunkManager = std::make_shared<PhysicsChunkManager>(filename, scaleFactor, 16);
    return mcc;
}

void MapChunkSystem::handleChunkUpdate(btScalar playerX, btScalar playerZ, MapChunkComponent& mcc) {
    mcc.physicsChunkManager->update(playerX, playerZ);
}