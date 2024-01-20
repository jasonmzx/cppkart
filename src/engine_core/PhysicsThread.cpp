#include "PhysicsThread.h"

PhysicsThread::PhysicsThread(SharedPhysicsResource* sharedResource) 
    : running(false), sharedRSRC(sharedResource) {
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

    //PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    while (running) {

        if (sharedRSRC) {
            vehiclePhysicsInfo vInfo;
            vInfo.test = 15;
            sharedRSRC->UpdateVehiclePhyInfo(vInfo);
        }
        
        uint8_t input;

        while (playerInputQueue.TryPop(input)) {

            // //TODO: Implement Cohesive Input Controller
            // if (input == 1) { //SDL_SCANCODE_UP
            //     vehiclePhysics.ApplyEngineForce(3500);
            // } else if (input == 2) { // SDL_SCANCODE_DOWN
            //     vehiclePhysics.ApplyEngineForce(-2500);
            // } else if (input == 0) {
            //     vehiclePhysics.ApplyEngineForce(0);
            // }
            
            // if (input == 3) { // SDL_SCANCODE_LEFT
            //     vehiclePhysics.ApplySteer(0.13);
            // } else if (input == 4) { // SDL_SCANCODE_RIGHT
            //     vehiclePhysics.ApplySteer(-0.13);
            // } else if (input == 5) {
            //     vehiclePhysics.ApplySteer(0);
            // }


            // Process and print the input command
            std::cout << "Input Received: " << static_cast<int>(input) << std::endl;
        }

        // std::cout << "Thread is running..." << std::endl;
        // Sleep or wait logic can be added here to control update rate
    }
}
