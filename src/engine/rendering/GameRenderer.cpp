#include "GameRenderer.hpp"


const std::string SHADER_PATH = "../src/game/shader/";

GameRenderer::GameRenderer(int winWidth, int winHeight, Camera* cam) 
{
  camera=cam;

  glViewport(0, 0, winWidth, winHeight); //(X1, Y1 bottom left) to (X2, Y2, top right)
  //glEnable(GL_DEPTH_TEST);

  glm::vec3 initCamLookAt = glm::vec3(1, 1, 1);
  camera->LookAt = initCamLookAt;

  // SHADER_PATH is a std::string
  std::string vertexShaderPath = SHADER_PATH + "default.vert";
  std::string fragmentShaderPath = SHADER_PATH + "default.frag";

  mainShader = std::make_unique<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  modelMatrixLOC = glGetUniformLocation(mainShader.get()->ID, "modelMatrix");
  useTextureLOC = glGetUniformLocation(mainShader.get()->ID, "useTexture");

}

void GameRenderer::RenderALL(SimulationWorld* world) {

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShader.get()->Activate();
}

void GameRenderer::renderObjects(const SimulationWorld* world) {

}

RenderList GameRenderer::createObjectRenderList(const SimulationWorld *world) {
    RenderList drawList;

    //Allocate size of drawList by doing world->Entities.size()

    // Potential Object Render class?


    return drawList;
}