#include "PhysicsThread.h"

PhysicsThread::PhysicsThread() : running(false) {
    // Constructor logic if needed
}

PhysicsThread::~PhysicsThread() {
    Stop(); // Ensure the thread is stopped on destruction
}

void PhysicsThread::Start(TSQueue<uint8_t>& playerInputQueue) {
    running = true;
    thread = std::thread(&PhysicsThread::ThreadLoop, this, std::ref(playerInputQueue));
}

void PhysicsThread::Stop() {
    running = false;
    if (thread.joinable()) {
        thread.join();
    }
}

void PhysicsThread::ThreadLoop(TSQueue<uint8_t>& playerInputQueue ) {
    while (running) {

        //PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
        
        uint8_t input;

        while (playerInputQueue.TryPop(input)) {
            // Process and print the input command
            std::cout << "Input Received: " << static_cast<int>(input) << std::endl;
        }

        // std::cout << "Thread is running..." << std::endl;
        // Sleep or wait logic can be added here to control update rate
    }
}
