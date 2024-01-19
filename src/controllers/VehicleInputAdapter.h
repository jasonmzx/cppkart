#ifndef VEHICLEINPUTADAPTER_CLASS_H
#define VEHICLEINPUTADAPTER_CLASS_H

#include <stdint.h>

#include "engine_core/PhysicsThread.h"

class VehicleInputAdapter {

    public:
        void vehicleKeyboardInput(const uint8_t *state);
        TSQueue<uint8_t> playerInputQueue;  
    private:
};

#endif