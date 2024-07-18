#ifndef PLAYERVEHICLECOMPONENT_HPP
#define PLAYERVEHICLECOMPONENT_HPP

#include "EComponent.hpp"
#include "jx_engine/physics/VehiclePhysics.hpp"
#include "jx_engine/io/GameInput.hpp"

#include "jx_engine/event/Event.hpp"
#include "jx_engine/logs/Logger.hpp"

#include <functional>

class PlayerVehicleComponent : public EComponent {

    public:
        
        PlayerVehicleComponent(float xPos, float yPos, float zPos);

        VehiclePhysics vehiclePhysics;

        void updateVehicleControl(GameInput::Control acceleration, GameInput::Control steer);

        void handlePlayerVehicleMoveEvent(const Event& event);

        void tick() override;

        void setPlayerPositionCallback(std::function<void(float, float, float, float)> callback);

    private:
        std::function<void(float, float, float, float)> setPlayerVehiclePosition;

};

#endif