#include "RenderComponent.hpp"

RenderComponent::RenderComponent(std::string modelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm)
{
    ressources = rrm;
    
    // modelPath = modelPath;
    vehicleChassisGeom = ressources->tryGetGeometry(modelPath);

    // texPath = texPath;
    texture = ressources->tryGetTex(texPath);
    if (texture == nullptr) { texture = ressources->loadTex(texPath, texPath, false); }
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
    texture.get()->Bind();
    vehicleChassisGeom->Draw(modelMatrixLOC, modelMatrix, colorUniformLocation, false);
    texture.get()->Unbind();
}