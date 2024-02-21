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

    auto simObj = std::make_shared<VehicleObject>("../src/ressources/first_car.obj", "../src/ressources/first_car_wheel.obj", "../src/ressources/first_car.png");

    simObj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

    auto simObj2 = std::make_shared<SimulationObject>("../src/ressources/box_1.obj", "../src/ressources/first_car.png");

    simObj2->position = glm::vec3(3.5f, 0.0f, 0.0f);
    simObj2->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    entities.push_back(simObj2);
}


