#include "SimulationWorld.hpp"

SimulationWorld::SimulationWorld() {


    //* Beta Testing 


   std::shared_ptr<ObjModel> carModel = std::make_shared<ObjModel>("../src/ressources/first_car.obj");
    
    std::shared_ptr<Texture> carTex = std::make_shared<Texture>("../src/ressources/first_car.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    auto simObj = std::make_shared<SimulationObject>(carModel, carTex);

    simObj->position = glm::vec3(0.0f, 0.0f, 0.0f);
    simObj->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    
    entities.push_back(simObj);

    std::shared_ptr<ObjModel> boxModel = std::make_shared<ObjModel>("../src/ressources/box_1.obj");

    // auto simObj2 = std::make_shared<SimulationObject>(boxModel);

    // simObj2->position = glm::vec3(3.5f, 0.0f, 0.0f);
    // simObj2->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // entities.push_back(simObj2);
}


