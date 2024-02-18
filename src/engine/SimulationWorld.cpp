#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld() {


    //* Beta Testing 

   std::shared_ptr<ObjModel> sharedModel = std::make_shared<ObjModel>("../src/ressources/first_car.obj");
    
    auto simObj = std::make_shared<SimulationObject>(sharedModel);

    simObj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

}


