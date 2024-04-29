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

    simObj = std::make_shared<VehicleObject>("../src/ressources/volga/volga.obj", "../src/ressources/first_car_wheel.obj", "../src/ressources/volga/volga.png");

    simObj->position = glm::vec3(50.0f, 0.0f, 0.0f);
    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

    auto simObj2 = std::make_shared<SimulationObject>("../src/ressources/Landscape01.obj", "../src/ressources/Map01_Albedo.png");

    //std::vector<LoadedChunk> chunks = ChunkedMapLoader::loadChunks("../src/ressources/chunk_map.txt");

    simObj2->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Set model matrix to 10x
    simObj2->objModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 40.0f, 40.0f));

    entities.push_back(simObj2);

}


void SimulationWorld::updateVehicleControls(GameInputState::Control accelerationControl, GameInputState::Control turnControl) {
    // Handle acceleration or braking based on the accelerationControl parameter
    switch (accelerationControl) {
        case GameInputState::VehicleAccelerate:
            simObj.get()->vehicle.ApplyEngineForce(2000);
            break;
        case GameInputState::VehicleBrake:
            simObj.get()->vehicle.ApplyEngineForce(-2500);
            break;
        default: // Covers GameInputState::Null and any other unspecified cases
            simObj.get()->vehicle.ApplyEngineForce(0);
            break;
    }

    // Handle turning based on the turnControl parameter
    switch (turnControl) {
        case GameInputState::VehicleTurnLeft:
            simObj.get()->vehicle.ApplySteer(0.13);
            break;
        case GameInputState::VehicleTurnRight:
            simObj.get()->vehicle.ApplySteer(-0.13);
            break;
        default: // Covers GameInputState::Null and any other unspecified cases
            simObj.get()->vehicle.ApplySteer(0);
            break;
    }
}