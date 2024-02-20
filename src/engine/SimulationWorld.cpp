#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld() {

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
    
    //* Beta Testing 

    auto simObj = std::make_shared<SimulationObject>("../src/ressources/first_car.obj", "../src/ressources/first_car.png");

    simObj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

    auto simObj2 = std::make_shared<SimulationObject>("../src/ressources/box_1.obj", "../src/ressources/first_car.png");

    simObj2->position = glm::vec3(3.5f, 0.0f, 0.0f);
    simObj2->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    entities.push_back(simObj2);
}


