#ifndef VEHICLEINPUTCONTROLLER_CLASS_H
#define VEHICLEINPUTCONTROLLER_CLASS_H

#include <stdint.h>

#include "engine_core/PhysicsThread.h"

class VehicleInputController {

    public:
        void vehicleKeyboardInput(const uint8_t *state);
        TSQueue<uint8_t> playerInputQueue;  
    private:
};

#endif