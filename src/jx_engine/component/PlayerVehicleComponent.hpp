#ifndef PLAYERVEHICLECOMPONENT_HPP
#define PLAYERVEHICLECOMPONENT_HPP

#include "EComponent.hpp"
#include "jx_engine/physics/VehiclePhysics.hpp"
#include "jx_engine/io/GameInput.hpp"

class PlayerVehicleComponent : public EComponent {

    public:
        PlayerVehicleComponent();

        VehiclePhysics vehiclePhysics;

        void updateVehicleControl(GameInput::Control acceleration, GameInput::Control steer);

};

#endif