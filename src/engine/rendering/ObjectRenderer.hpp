#ifndef OBJECTRENDERER_hpp
#define OBJECTRENDERER_hpp

#include <memory>
#include "engine/SimulationWorld.hpp"
#include "engine/objects/VehicleObject.hpp"

#include "RenderRsrcManager.hpp"

class ObjectRenderer {

    public:
    ObjectRenderer(SimulationWorld* worldArg, RenderRsrcManager* rsrc, std::shared_ptr<Shader> shader)
        : world(worldArg), ressources(rsrc), gameShader(shader) {}

        void addToBuildlist(SimulationObject* simObj, RenderList& renderList);

        void renderObject(SimulationObject* obj, RenderList& renderList);
        void renderVehicle(VehicleObject* vehicleObj, RenderList& renderList);
    private:

        RenderRsrcManager* ressources;
        SimulationWorld* world;

        std::shared_ptr<Shader> gameShader;
};

#endif