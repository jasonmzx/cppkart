#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld() {

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();
    
    //* Beta Testing 


   std::shared_ptr<ObjModel> carModel = std::make_shared<ObjModel>("../src/ressources/first_car.obj");
    
    auto simObj = std::make_shared<SimulationObject>(carModel, "../src/ressources/first_car.png");

    simObj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

    std::shared_ptr<ObjModel> boxModel = std::make_shared<ObjModel>("../src/ressources/box_1.obj");

    auto simObj2 = std::make_shared<SimulationObject>(boxModel, "../src/ressources/first_car.png");

    simObj2->position = glm::vec3(3.5f, 0.0f, 0.0f);
    simObj2->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    entities.push_back(simObj2);
}


