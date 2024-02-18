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


  VAO1.Bind();
  auto drawList = createObjectRenderList(world);

}

void GameRenderer::RenderALL() {

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShader.get()->Activate();
}

void GameRenderer::renderObjects() {


  // for (const auto& instruction : drawList) {
  //       // glBindVertexArray(instruction.VertexArrayObject->ID);
  //       // // Set uniforms like modelMatrix here
  //       // glUniformMatrix4fv(modelMatrixLOC, 1, GL_FALSE, glm::value_ptr(instruction.modelMatrix));
  //       // //instruction.VertexArrayObject->meshes[0].Draw(); // Draw the mesh
  // }
}

RenderList GameRenderer::createObjectRenderList(const SimulationWorld *world) {
    RenderList drawList;
    
    for (auto& entity : world->entities) {
        RenderInstruction instruction;
        
        //! Crashes
        // instruction.modelMatrix = glm::translate(glm::mat4(1.0f), entity->getPosition()) * glm::toMat4(entity->getRotation());
        
        // std::vector<GLfloat> verts = entity->model->GetVertices();
        // std::vector<GLuint> indices = entity->model->GetIndices();

        //instruction.geometry(&VA);

        //RenderableGeometry roadGeom(&VAO5, &VBO5, &EBO5, roadVertices, roadIndices);



        drawList.push_back(instruction);
    }
    return drawList;
}