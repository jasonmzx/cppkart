#include "GameRenderer.hpp"


#define BULLET_DEBUG_DRAW 1

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
    mainShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    // Get locations of uniforms in the shader
    modelMatrixLOC = glGetUniformLocation(mainShader.get()->ID, "modelMatrix");
    useTextureLOC = glGetUniformLocation(mainShader.get()->ID, "useTexture");

    colorUniformLocation = glGetUniformLocation(mainShader.get()->ID, "FragColor");

    // Initialize Object Renderer
    objectRender = std::make_unique<ObjectRenderer>(world, &ressources, mainShader);
      
    if(BULLET_DEBUG_DRAW == 1)
      debugDrawer = new BulletDebugDrawer(mainShader.get()->ID);
      
    world->physicsWorld->dynamicsWorld->setDebugDrawer(debugDrawer);

}

void GameRenderer::RenderALL() {

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mainShader.get()->Activate();




    glUniform1i(useTextureLOC, GL_FALSE); 
        
        world->physicsWorld->dynamicsWorld->debugDrawWorld();
        debugDrawer->flushLines();

    glUniform1i(useTextureLOC, GL_TRUE); 


    renderObjects();

    camera->Matrix(45.0f, 0.1f, 1000.0f, mainShader, "camMatrix"); //! IMPORTANT

}

void GameRenderer::renderObjects() {
   //terrainGeom.Draw(modelMatrixLocation, terrainModelMatrix, colorUniformLocation, false);
    
    ressources.debugPrint();
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
    
    for (auto& entity : world->entities) { //Simulation object entities


      objectRender.get()->addToBuildlist(entity.get(), drawList);

        //Uncooment below blck

        // RenderInstruction instruction;
        
        // std::string modelID = entity->modelPath; //entity's primary modelpath String ID

        // //* ================= Geometry ====================

        // auto geometry = ressources.tryGetGeometry(modelID);

        // if(geometry == nullptr) { //If geometry doesn't exist, make it
        
        //   //Load Model:
        //   auto model = ressources.tryGetModel(modelID);
        //   if(model == nullptr) {
        //     model = ressources.loadModel(modelID, modelID);
        //   }

        //   printf("New Model Loaded!");

        //   std::vector<GLfloat> verts = model->GetVertices();
        //   std::vector<GLuint> indices = model->GetIndices();
        //   geometry = ressources.getOrCreateGeometry(modelID, model->GetVertices(), model->GetIndices());
        // }

        // //* ================= Tex ====================

        // //TODO: Maybe Hash This into a smaller ID?
        // std::string texID = entity->texPath; //Texture's String ID

        // auto texture = ressources.tryGetTex(texID);

        // if(texture == nullptr){
        //   instruction.tex.get()->texUnit(mainShader, "tex0", 0);
        //   texture = ressources.loadTex(texID, entity.get()->texPath);  
        // }
        // instruction.tex = texture;
        // instruction.geometry = geometry;

        // glm::vec3 scaleFactors = glm::vec3(1.0f, 1.0f, 1.0f); // Example scale factors, adjust as necessary
        // instruction.modelMatrix = glm::translate(glm::mat4(1.0f), entity->getPosition())
        //                           * glm::mat4_cast(entity->getRotation()) // Convert quaternion to rotation matrix
        //                           * glm::scale(glm::mat4(1.0f), scaleFactors);

        // drawList.push_back(instruction);
    }
    return drawList;
}

