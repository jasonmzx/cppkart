#include "RenderComponent.hpp"

//Logger* logger = Logger::getInstance();

RenderComponent::RenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, bool cD, bool isTexAlpha)
{

    ressources = RenderRsrcManager::getInstance();

    GameGLRenderer* renderer = GameGLRenderer::getInstance();

    SetGLContext(renderer->useTextureLOC, renderer->modelMatrixLOC, renderer->colorUniformLocation);

    Logger* logger = Logger::getInstance();

    auto chunk_loading_start = std::chrono::high_resolution_clock::now();

    cullDisable = !cD;

    // modelPath = modelPath;
    Geom = ressources->tryGetCombinedGeometry(modelPath, meshIndices);

    // texPath = texPath;
    Tex = ressources->tryGetTex(texPath);
    if (Tex == nullptr) { Tex = ressources->loadTex(texPath, texPath, isTexAlpha); }

    auto chunk_loading_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> chunk_loading_time = chunk_loading_end - chunk_loading_start;

    logger->log(Logger::TIMER, "[RENDER COMPONENT] : Total Chunk Loading Time: " + std::to_string(chunk_loading_time.count()) + "sec(s) | \n >>> Mesh: " + modelPath + " \n Tex " + texPath);
}

void RenderComponent::SetGLContext(GLint texLOCATION, GLint mmLOCATION, GLint colorUniformLOCATION)
{    
    useTextureLOC = texLOCATION;
    modelMatrixLOC = mmLOCATION;
    colorUniformLOC = colorUniformLOCATION;
}

void RenderComponent::SetRenderScale(float scale)
{
    ObjmodelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
}

void printMatrix(const glm::mat4& matrix) {
    const float* ptr = glm::value_ptr(matrix);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << ptr[j * 4 + i] << " ";
        }
        std::cout << std::endl;
    }
}

void RenderComponent::Draw()
{
    // Log the model matrix for debugging
    // logger->log(Logger::INFO, "Model Matrix: ");
    // printMatrix(ObjmodelMatrix);

    // Save the current culling state
    GLboolean cullingEnabled = glIsEnabled(GL_CULL_FACE);
    
    glGetIntegerv(GL_CULL_FACE_MODE, &currentCullFace);
    glGetIntegerv(GL_FRONT_FACE, &currentFrontFace);

    // Temporarily disable face culling if cullDirection is false
    if (cullDisable) {
        glDisable(GL_CULL_FACE);
    }

    Tex.get()->Bind();
    Geom->Draw(modelMatrixLOC, ObjmodelMatrix, colorUniformLOC, false);
    Tex.get()->Unbind();

    // Restore the original culling state
    glEnable(GL_CULL_FACE);
    glCullFace(currentCullFace);
    glFrontFace(currentFrontFace);

}

std::vector<glm::vec3> RenderComponent::GetXYZvertices()
{
    return Geom->GetXYZvertices();
}

void RenderComponent::tick()
{
    Draw();
}