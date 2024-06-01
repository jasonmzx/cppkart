#include "RenderComponent.hpp"

RenderComponent::RenderComponent(std::string modelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm)
{
    ressources = rrm;
    
    // modelPath = modelPath;
    Geom = ressources->tryGetGeometry(modelPath);

    // texPath = texPath;
    Tex = ressources->tryGetTex(texPath);
    if (Tex == nullptr) { Tex = ressources->loadTex(texPath, texPath, false); }
}

void RenderComponent::SetGLContext(GLint useTextureLOC, GLint modelMatrixLOC, GLint colorUniformLocation)
{
    useTextureLOC = useTextureLOC;
    modelMatrixLOC = modelMatrixLOC;
    colorUniformLocation = colorUniformLocation;

    modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(40.0f, 40.0f, 40.0f));
}

void RenderComponent::Draw()
{
    Tex.get()->Bind();
    Geom->Draw(modelMatrixLOC, modelMatrix, colorUniformLocation, false);
    Tex.get()->Unbind();
}