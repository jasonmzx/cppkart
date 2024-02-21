#include "ObjectRenderer.hpp"


void ObjectRenderer::renderObject(SimulationObject* obj, RenderList& renderList) {
    
    RenderInstruction instruction;

    auto geometry = ressources->tryGetGeometry(obj->modelPath);

    if(geometry == nullptr) { //If geometry doesn't exist, make it
        
          //Load Model:
          auto model = ressources->tryGetModel(obj->modelPath);
          if(model == nullptr) {
            model = ressources->loadModel(obj->modelPath, obj->modelPath);
          }

          printf("New Model Loaded!");

          std::vector<GLfloat> verts = model->GetVertices();
          std::vector<GLuint> indices = model->GetIndices();
          geometry = ressources->getOrCreateGeometry(obj->modelPath, model->GetVertices(), model->GetIndices());
    }

            //* ================= Tex ====================

        //TODO: Maybe Hash This into a smaller ID?
        std::string texID = obj->texPath; //Texture's String ID

        auto texture = ressources->tryGetTex(texID);

        if(texture == nullptr){
          instruction.tex.get()->texUnit(gameShader, "tex0", 0);
          texture = ressources->loadTex(texID, texID);  
        }
        instruction.tex = texture;
        instruction.geometry = geometry;

        glm::vec3 scaleFactors = glm::vec3(1.0f, 1.0f, 1.0f); // Example scale factors, adjust as necessary
        instruction.modelMatrix = glm::translate(glm::mat4(1.0f), obj->getPosition())
                                  * glm::mat4_cast(obj->getRotation()) // Convert quaternion to rotation matrix
                                  * glm::scale(glm::mat4(1.0f), scaleFactors);
        
        renderList.push_back(instruction);

}

void ObjectRenderer::renderVehicle(VehicleObject* vehicleObj, RenderList& renderList) {
    
    RenderInstruction instruction;

    auto geometry = ressources->tryGetGeometry(vehicleObj->modelPath);

    if(geometry == nullptr) { //If geometry doesn't exist, make it
        
          //Load Model:
          auto model = ressources->tryGetModel(vehicleObj->modelPath);
          if(model == nullptr) {
            model = ressources->loadModel(vehicleObj->modelPath, vehicleObj->modelPath);
          }

          printf("New Model Loaded!");

          std::vector<GLfloat> verts = model->GetVertices();
          std::vector<GLuint> indices = model->GetIndices();
          geometry = ressources->getOrCreateGeometry(vehicleObj->modelPath, model->GetVertices(), model->GetIndices());
    }

            //* ================= Tex ====================

        //TODO: Maybe Hash This into a smaller ID?
        std::string texID = vehicleObj->texPath; //Texture's String ID

        auto texture = ressources->tryGetTex(texID);

        if(texture == nullptr){
          instruction.tex.get()->texUnit(gameShader, "tex0", 0);
          texture = ressources->loadTex(texID, texID);  
        }
        instruction.tex = texture;
        instruction.geometry = geometry;

         vehicleObj->UpdateModelMatrix();
        instruction.modelMatrix = vehicleObj->objModelMatrix;
        
        renderList.push_back(instruction);

}


void ObjectRenderer::addToBuildlist(SimulationObject* simObj, RenderList& renderList){

    switch(simObj->type) {
        case SimulationObject::Vehicle:
            renderVehicle(static_cast<VehicleObject*>(simObj), renderList);
            break;

        default:
            renderObject(simObj, renderList); 
            break;
    }
}