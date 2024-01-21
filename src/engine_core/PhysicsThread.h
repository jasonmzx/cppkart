#ifndef PHYSICSTHREAD_CLASS_H
#define PHYSICSTHREAD_CLASS_H

#include <thread>
#include <atomic>
#include <iostream>

// Core Imports
#include "singletons/PhysicsWorldSingleton.h"

#include "bullet_models/VehiclePhysics.h"

#include "managers/PhysicsChunkManager.h"

#include "./data_structs/TSQueue.cpp"

#include "shared_resources/SharedPhysicsResource.h"

#include "engine_core/geometries/BulletDebugDrawer.cpp"

class PhysicsThread {
public:
     PhysicsThread(SharedPhysicsResource* sharedResource, BulletDebugDrawer* debugDrawer);
    ~PhysicsThread();
    

    void Start(TSQueue<uint8_t>& playerInputQueue);
    void Stop();


private:
    void ThreadLoop(TSQueue<uint8_t>& playerInputQueue);
    std::thread thread;
    std::atomic<bool> running;

    SharedPhysicsResource* sharedRSRC;  // Pointer to shared ressource with main thread

    //* Actual Physics Element in the World
    VehiclePhysics vehiclePhysics;
    BulletDebugDrawer* debugDrawer; 

};

#endif
