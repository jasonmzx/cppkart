#include "PhysicsThread.h"

#include <chrono>

PhysicsThread::PhysicsThread(SharedPhysicsResource* sharedResource, BulletDebugDrawer* debugDrawer)
    : running(false), sharedRSRC(sharedResource), debugDrawer(debugDrawer) { // Modify this line
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

    auto lastTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;
    const double physicsTimestep = 1.0 / 60.0;

    if (debugDrawer) {
            physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);
            sharedRSRC->UpdatePhysicsWorld(physicsWorld->dynamicsWorld);
    }

    while (running) {
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        double frameTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastTime).count();
        lastTime = currentTime;
        accumulator += frameTime;

        // while (accumulator >= physicsTimestep) {
        // physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);
        

        // }

        // //physicsWorld->dynamicsWorld->debugDrawWorld();

        // auto startTime = std::chrono::high_resolution_clock::now();

        // btTransform vehicleTransform = vehiclePhysics.GetTransform();

        // if (sharedRSRC) {
        //     vehiclePhysicsInfo vInfo;
        //     vInfo.transform = vehicleTransform;
        //     sharedRSRC->UpdateVehiclePhyInfo(vInfo);
        //     sharedRSRC->SwapBuffers();

        //    // sharedRSRC->UpdatePhysicsWorld(physicsWorld->dynamicsWorld);
        // }
        
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

                // Update physics as many times as required by the accumulated time
        while (accumulator >= physicsTimestep) {
            // Step the physics simulation
            physicsWorld->dynamicsWorld->stepSimulation(physicsTimestep);

            // Get the latest vehicle transform and update shared resource
            btTransform vehicleTransform = vehiclePhysics.GetTransform();
            if (sharedRSRC) {
                vehiclePhysicsInfo vInfo;
                vInfo.transform = vehicleTransform;
                sharedRSRC->UpdateVehiclePhyInfo(vInfo);
                sharedRSRC->SwapBuffers();
            }

            // Decrease the accumulated time
            accumulator -= physicsTimestep;
        }

        btVector3 vehiclePosition = vehiclePhysics.GetTransform().getOrigin();
        
        //player positions (vehicle)
        
        btScalar pXpos = vehiclePosition.getX();
        btScalar pYpos = vehiclePosition.getY();
        btScalar pZpos = vehiclePosition.getZ();

        terrainChunkManager.update(pXpos, pZpos);
        
        
        // std::cout << "Thread is running..." << std::endl;
        // Sleep or wait logic can be added here to control update rate
    


    }
}
