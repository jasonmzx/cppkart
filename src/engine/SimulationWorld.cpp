#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld() {


    //* Beta Testing 

   std::shared_ptr<ObjModel> sharedModel = std::make_shared<ObjModel>("../src/ressources/first_car.obj");
    
    auto obj = std::make_shared<SimulationObject>(sharedModel);

    SimulationObject* objPtr = obj.get();

    obj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    obj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(objPtr);

}


