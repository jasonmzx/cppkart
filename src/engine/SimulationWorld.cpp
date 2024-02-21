#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld() {

    physicsWorld = PhysicsWorldSingleton::getInstance();

     //* ############ PROTOTYPE Collision Plane ############

  btTransform protoPlaneTransform;
  protoPlaneTransform.setIdentity();
  protoPlaneTransform.setOrigin(btVector3(0, 0, 0));
  btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));

  // Create Motion shape:
  btMotionState *motion = new btDefaultMotionState(protoPlaneTransform); //! He put btDefaultMotionShape

  btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
  info.m_friction = 2.0f;

  btRigidBody *planeBody = new btRigidBody(info);
  physicsWorld->dynamicsWorld->addRigidBody(planeBody);
  

  //* ############ PROTOTYPE Collision Plane ^^^ ############
    
    //* Beta Testing 

    simObj = std::make_shared<VehicleObject>("../src/ressources/first_car.obj", "../src/ressources/first_car_wheel.obj", "../src/ressources/first_car.png");

    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

    auto simObj2 = std::make_shared<SimulationObject>("../src/ressources/box_1.obj", "../src/ressources/first_car.png");

    simObj2->position = glm::vec3(3.5f, 0.0f, 0.0f);
    simObj2->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    entities.push_back(simObj2);
}


void SimulationWorld::updateWithPlayerInput(const ActiveInput& inputs) {
    for (const auto& input : inputs) {
        switch (input) {
            case GameInputState::VehicleAccelerate:
                printf("Trying to hit the gass!");
                simObj->vehicle.ApplyEngineForce(1000); // Example force, adjust as needed
                break;
            case GameInputState::VehicleBrake:
                simObj->vehicle.Brake(100); // Example brake force
                break;
            case GameInputState::VehicleTurnLeft:
                simObj->vehicle.ApplySteer(-0.1); // Example steering increment, adjust as needed
                break;
            case GameInputState::VehicleTurnRight:
                simObj->vehicle.ApplySteer(0.1); // Example steering increment
                break;
            // Handle other inputs as needed
        }
    }
}