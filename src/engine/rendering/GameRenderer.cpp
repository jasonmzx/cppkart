#include "GameRenderer.hpp"


const std::string SHADER_PATH = "../src/game/shader/";

GameRenderer::GameRenderer(int winWidth, int winHeight, Camera* cam, SimulationWorld* worldArg)
: camera(cam), world(worldArg) { // world is now a raw pointer

    glViewport(0, 0, winWidth, winHeight); // Set viewport size

    // Initialize the camera's look-at vector
    glm::vec3 initCamLookAt = glm::vec3(1, 1, 1);
    camera->LookAt = initCamLookAt;

    // Initialize shaders
    std::string vertexShaderPath = SHADER_PATH + "default.vert";
    std::string fragmentShaderPath = SHADER_PATH + "default.frag";
    mainShader = std::make_unique<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    // Get locations of uniforms in the shader
    modelMatrixLOC = glGetUniformLocation(mainShader.get()->ID, "modelMatrix");
    useTextureLOC = glGetUniformLocation(mainShader.get()->ID, "useTexture");

    colorUniformLocation = glGetUniformLocation(mainShader.get()->ID, "FragColor");

    //Initialize Textures:

}

void GameRenderer::RenderALL() {

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShader.get()->Activate();

    renderObjects();

    camera->Matrix(45.0f, 0.1f, 1000.0f, mainShader, "camMatrix"); //! IMPORTANT

}

void GameRenderer::renderObjects() {
   //terrainGeom.Draw(modelMatrixLocation, terrainModelMatrix, colorUniformLocation, false);
    
auto drawList = createObjectRenderList();

    glUniform1i(useTextureLOC, GL_TRUE); 

  for (const auto& instruction : drawList) {

    instruction.tex.get()->Bind();

    glm::mat4 tempMatrix = instruction.modelMatrix; // Create a non-const copy
      instruction.geometry->Draw(modelMatrixLOC, tempMatrix, colorUniformLocation, false);
    
    instruction.tex.get()->Unbind();
  }

}

RenderList GameRenderer::createObjectRenderList() {
    RenderList drawList;
    
    for (auto& entity : world->entities) {
        RenderInstruction instruction;
        
        std::string modelIdentifier = entity->model->modelID;
        
        std::vector<GLfloat> verts = entity->model->GetVertices();
        std::vector<GLuint> indices = entity->model->GetIndices();
        
        //! THIS IS TERRIBLE AS EVERY DRAW ITERATION IT INITS ! NO GOOD
        instruction.tex = entity->tex0;
        instruction.tex.get()->texUnit(mainShader, "tex0", 0);

        auto geometry = getOrCreateGeometry(modelIdentifier, entity->model->GetVertices(), entity->model->GetIndices());
        instruction.geometry = geometry;

        glm::vec3 scaleFactors = glm::vec3(1.0f, 1.0f, 1.0f); // Example scale factors, adjust as necessary
        instruction.modelMatrix = glm::translate(glm::mat4(1.0f), entity->getPosition())
                                  * glm::mat4_cast(entity->getRotation()) // Convert quaternion to rotation matrix
                                  * glm::scale(glm::mat4(1.0f), scaleFactors);

        drawList.push_back(instruction);
    }
    return drawList;
}

std::shared_ptr<Geometry> GameRenderer::getOrCreateGeometry(const std::string& modelIdentifier, 
const std::vector<GLfloat>& verts, const std::vector<GLuint>& indices) {

    auto iter = geometryCache.find(modelIdentifier);
    if (iter != geometryCache.end()) {
        // Found existing geometry, reuse it
        return iter->second;
    } else {
        // Create new Geometry object
        auto geometry = std::make_shared<Geometry>(verts, indices);
        geometryCache[modelIdentifier] = geometry;
        return geometry;
    }
}