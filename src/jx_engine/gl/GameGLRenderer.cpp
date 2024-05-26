#include "GameGLRenderer.hpp"
#include <chrono>

#define BULLET_DEBUG_DRAW 1

const std::string SHADER_PATH = "../src/game/shader/";

GameGLRenderer::GameGLRenderer(int winWidth, int winHeight, Camera *cam) : camera(cam)
{ 
  glViewport(0, 0, winWidth, winHeight); // Set viewport size

  // Initialize the camera's look-at vector
  glm::vec3 initCamLookAt = glm::vec3(1, 1, 1);
  camera->LookAt = initCamLookAt;

  // Initialize shaders
  std::string vertexShaderPath = SHADER_PATH + "default.vert";
  std::string fragmentShaderPath = SHADER_PATH + "default.frag";
  mainShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

  // Get locations of uniforms in the shader
  modelMatrixLOC = glGetUniformLocation(mainShader.get()->ID, "modelMatrix");
  useTextureLOC = glGetUniformLocation(mainShader.get()->ID, "useTexture");


  colorUniformLocation = glGetUniformLocation(mainShader.get()->ID, "FragColor");

  if (BULLET_DEBUG_DRAW == 1)
    debugDrawer = new BulletDebugDrawer(mainShader.get()->ID); //TODO: Fix mem leak (dealloc this somewhere)

  //TODO: Find out how physicsWorld is initialized  
  //world->physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);
}
