#include "PhysicsThread.h"

#include <chrono>

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

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
    PhysicsChunkManager terrainChunkManager("../src/ressources/Map_1K.png");

        // Duration of each tick (1/60th of a second)
    auto tickDuration = std::chrono::milliseconds(1000 / 120);

    while (running) {
        auto startTime = std::chrono::high_resolution_clock::now();

        btTransform vehicleTransform = vehiclePhysics.GetTransform();

        if (sharedRSRC) {
            vehiclePhysicsInfo vInfo;
            vInfo.transform = vehicleTransform;
            sharedRSRC->UpdateVehiclePhyInfo(vInfo);
        }
        
        uint8_t input;

        while (playerInputQueue.TryPop(input)) {
            
            auto startTime = std::chrono::high_resolution_clock::now();

            // //TODO: Implement Cohesive Input Controller
            if (input == 1) { //SDL_SCANCODE_UP
                vehiclePhysics.ApplyEngineForce(3500);
            } else if (input == 2) { // SDL_SCANCODE_DOWN
                vehiclePhysics.ApplyEngineForce(-2500);
            } else if (input == 0) {
                vehiclePhysics.ApplyEngineForce(0);
            }
            
            if (input == 3) { // SDL_SCANCODE_LEFT
                vehiclePhysics.ApplySteer(0.13);
            } else if (input == 4) { // SDL_SCANCODE_RIGHT
                vehiclePhysics.ApplySteer(-0.13);
            } else if (input == 5) {
                vehiclePhysics.ApplySteer(0);
            }


            // Process and print the input command
            std::cout << "Input Received: " << static_cast<int>(input) << std::endl;
        }


        btVector3 vehiclePosition = vehicleTransform.getOrigin();
        
            //player positions (vehicle)
        btScalar pXpos = vehiclePosition.getX();
        btScalar pYpos = vehiclePosition.getY();
        btScalar pZpos = vehiclePosition.getZ();

        terrainChunkManager.update(pXpos, pZpos);
        
        physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);
        
        // std::cout << "Thread is running..." << std::endl;
        // Sleep or wait logic can be added here to control update rate
    
            // Calculate the duration of the loop execution
        auto endTime = std::chrono::high_resolution_clock::now();
        auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        // Sleep if the loop executed faster than the tick duration
        if (loopDuration < tickDuration) {
            std::this_thread::sleep_for(tickDuration - loopDuration);
        }
    
    }
}
