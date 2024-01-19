#ifndef PHYSICSTHREAD_CLASS_H
#define PHYSICSTHREAD_CLASS_H

#include <thread>
#include <atomic>
#include <iostream>

// Core Imports
#include "singletons/PhysicsWorldSingleton.h"

#include "bullet_models/VehiclePhysics.h"

#include "./data_structs/TSQueue.cpp"

class PhysicsThread {
public:
    PhysicsThread();
    ~PhysicsThread();
    

    void Start(TSQueue<uint8_t>& playerInputQueue);
    void Stop();

private:
    void ThreadLoop(TSQueue<uint8_t>& playerInputQueue);
    std::thread thread;
    std::atomic<bool> running;

    //* Actual Physics Element in the World
    //VehiclePhysics vehiclePhysics;

};

#endif
