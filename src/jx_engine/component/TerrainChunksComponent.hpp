#ifndef TERRAINCHUNKSCOMPONENT_HPP
#define TERRAINCHUNKSCOMPONENT_HPP

#include "EComponent.hpp"

#include "jx_engine/physics/manager/PhysicsChunkManager.hpp"
#include "jx_engine/logs/Logger.hpp"

class TerrainChunksComponent : public EComponent {

    public:
        std::shared_ptr<PhysicsChunkManager> physicsChunkManager;
        TerrainChunksComponent(const std::string& filename, float scaleFactor);
        void updateChunks(btScalar playerX, btScalar playerZ);
        int ticker_counter = 0;

};

#endif