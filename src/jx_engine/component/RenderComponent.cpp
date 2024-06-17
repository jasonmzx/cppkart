#include "RenderComponent.hpp"

RenderComponent::RenderComponent(std::string modelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm, int meshIndex, bool cD)
{
    cullDisable = !cD;
    ressources = rrm;
    
    // modelPath = modelPath;
    Geom = ressources->tryGetGeometry(modelPath, meshIndex);

    // texPath = texPath;
    Tex = ressources->tryGetTex(texPath);
    if (Tex == nullptr) { Tex = ressources->loadTex(texPath, texPath, false); }
}

void RenderComponent::SetGLContext(GLint texLOCATION, GLint mmLOCATION, GLint colorUniformLOCATION, float scale)
{
    useTextureLOC = texLOCATION;
    modelMatrixLOC = mmLOCATION;
    colorUniformLOC = colorUniformLOCATION;

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

