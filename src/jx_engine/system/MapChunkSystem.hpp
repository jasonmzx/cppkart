#ifndef MAPCHUNKSYSTEM_HPP
#define MAPCHUNKSYSTEM_HPP

#include <memory>

#include "System.hpp"

#include "jx_engine/physics/manager/PhysicsChunkManager.hpp"

struct MapChunkComponent {

    std::shared_ptr<PhysicsChunkManager> physicsChunkManager;

};

class MapChunkSystem : public System {
    
    public:
        MapChunkComponent buildMapChunkComponent(const std::string& filename, float scaleFactor);
        void handleChunkUpdate(btScalar playerX, btScalar playerZ, MapChunkComponent& mcc);
};



#endif  // MAPCHUNKSYSTEM_HPP