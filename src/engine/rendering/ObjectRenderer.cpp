#include "ObjectRenderer.hpp"

void ObjectRenderer::renderObject(SimulationObject *obj, RenderList &renderList)
{

  RenderInstruction instruction;
  auto geometry = ressources->tryGetGeometry(obj->modelPath);

  //* ================= Texturing ====================

  std::string texID = obj->texPath; // Texture's String ID
  auto texture = ressources->tryGetTex(texID);

  if (texture == nullptr)
  {
    instruction.tex.get()->texUnit(gameShader, "tex0", 0);
    texture = ressources->loadTex(texID, texID, false);
  }

  //* ================= Build Instruction ====================

  instruction.tex = texture;
  instruction.geometry = geometry;

  glm::vec3 scaleFactors = glm::vec3(1.0f, 1.0f, 1.0f); // Example scale factors, adjust as necessary

  //instruction.modelMatrix = glm::translate(glm::mat4(1.0f), obj->getPosition()) * glm::mat4_cast(obj->getRotation()) // Convert quaternion to rotation matrix
  //                          * glm::scale(glm::mat4(1.0f), scaleFactors);

  instruction.modelMatrix = obj->objModelMatrix;

  renderList.push_back(instruction);
}

void ObjectRenderer::renderVehicle(VehicleObject *vehicleObj, RenderList &renderList)
{

  vehicleObj->UpdateModelMatrix(); //Update all transforms before rendering (Body and wheels)

  RenderInstruction instruction;
  
  auto vehicleGeometry = ressources->tryGetGeometry(vehicleObj->modelPath);

  auto wheelGeometry = ressources->tryGetGeometry(vehicleObj->wheelObjPath);



  //* ================= Texturing ====================

  std::string texID = vehicleObj->texPath; // Texture's String ID

  auto texture = ressources->tryGetTex(texID);

  if (texture == nullptr)
  {
    instruction.tex.get()->texUnit(gameShader, "tex0", 0);
    texture = ressources->loadTex(texID, texID, true);
  }
  
  //* ================= Build Instructions ====================

  instruction.tex = texture;
  instruction.geometry = vehicleGeometry;

  instruction.modelMatrix = vehicleObj->objModelMatrix;

  renderList.push_back(instruction);

  //* ================= Wheel Instructions ====================

  for (glm::mat4 wheelMatrix : vehicleObj->wheelMatrices)
  {
    RenderInstruction wheelInstruction;
    wheelInstruction.tex = texture;
    wheelInstruction.geometry = wheelGeometry;
    wheelInstruction.modelMatrix = wheelMatrix;

    renderList.push_back(wheelInstruction);
  }
}

void ObjectRenderer::addToBuildlist(SimulationObject *simObj, RenderList &renderList)
{

  switch (simObj->type)
  {
  case SimulationObject::Vehicle:
    renderVehicle(static_cast<VehicleObject *>(simObj), renderList);
    break;

  default:
    renderObject(simObj, renderList);
    break;
  }
}