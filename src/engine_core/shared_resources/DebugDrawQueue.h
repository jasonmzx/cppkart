#ifndef DEBUGDRAWQUEUE_CLASS_H
#define DEBUGDRAWQUEUE_CLASS_H

#include "engine_core/PhysicsThread.h"

class DebugDrawRSRC_Queue {

    public:
        TSQueue<uint8_t> playerInputQueue;  
    private:
};

#endif