#ifndef PHYSICSTHREAD_CLASS_H
#define PHYSICSTHREAD_CLASS_H

#include <thread>
#include <atomic>
#include <iostream>

// Core Imports
#include "singletons/PhysicsWorldSingleton.h"

class PhysicsThread {
public:
    PhysicsThread();
    ~PhysicsThread();

    void Start();
    void Stop();

private:
    void ThreadLoop();
    std::thread thread;
    std::atomic<bool> running;
};

#endif
