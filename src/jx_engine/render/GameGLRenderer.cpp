#include "GameGLRenderer.hpp"
#include <chrono>

#define BULLET_DEBUG_DRAW 1

const std::string SHADER_PATH = "../src/game/shader/";

GameGLRenderer::GameGLRenderer(int winWidth, int winHeight, Camera *cam) : camera(cam)
{ 
  physicsWorld = PhysicsWorldSingleton::getInstance();
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
    printf("Debug draw enabled\n");
    physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);

  //TODO: Find out how physicsWorld is initialized  
  //world->physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);

  std::cout << "modelMatrixLOC: " << modelMatrixLOC << std::endl;
  std::cout << "useTextureLOC: " << useTextureLOC << std::endl;
  std::cout << "colorUniformLocation: " << colorUniformLocation << std::endl;

}

void GameGLRenderer::RenderPrep() {

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShader->Activate();

    glUniform1i(useTextureLOC, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

void GameGLRenderer::DebugRender() {
    if (BULLET_DEBUG_DRAW == 1) {
        glUniform1i(useTextureLOC, GL_FALSE);

        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(modelMatrixLOC, 1, GL_FALSE, glm::value_ptr(identityMatrix));

        physicsWorld->dynamicsWorld->debugDrawWorld();
        debugDrawer->flushLines();
        glUniform1i(useTextureLOC, GL_TRUE);
    }
}