#include "PhysicsThread.h"

PhysicsThread::PhysicsThread() : running(false) {
    // Constructor logic if needed
}

PhysicsThread::~PhysicsThread() {
    Stop(); // Ensure the thread is stopped on destruction
}

void PhysicsThread::Start() {
    running = true;
    thread = std::thread(&PhysicsThread::ThreadLoop, this);
}

void PhysicsThread::Stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void PhysicsThread::ThreadLoop() {
    while (running) {

        //PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();


        std::cout << "Thread is running..." << std::endl;
        // Sleep or wait logic can be added here to control update rate
    }
}
