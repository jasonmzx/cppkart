#include "RenderSystem.hpp"

//Logger* logger = Logger::getInstance();

RenderSystem::RenderSystem()
{
    renderer = GameGLRenderer::getInstance();
    SetGLContext(renderer->useTextureLOC, renderer->modelMatrixLOC, renderer->colorUniformLocation);
}

RenderComponent RenderSystem::buildRenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, float scale, bool cD, bool isTexAlpha)
{

    ressources = RenderRsrcManager::getInstance();
    renderer = GameGLRenderer::getInstance();

    // Init RenderComponent with the given parameters

    RenderComponent rc;

    rc.modelPath = modelPath;
    rc.Geom = ressources->tryGetCombinedGeometry(modelPath, meshIndices);

    rc.texPath = texPath;
    rc.Tex = ressources->tryGetTex(texPath);

    rc.modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));

    rc.cullDisable = !cD;

    if (rc.Tex == nullptr) { rc.Tex = ressources->loadTex(texPath, texPath, isTexAlpha); }

    return rc;
}

void RenderSystem::SetGLContext(GLint texLOCATION, GLint mmLOCATION, GLint colorUniformLOCATION)
{    
    useTextureLOC = texLOCATION;
    modelMatrixLOC = mmLOCATION;
    colorUniformLOC = colorUniformLOCATION;
}

void RenderSystem::Draw(RenderComponent& rc)
{
    // Ensure the geometry and texture are valid
    if (rc.Geom == nullptr || rc.Tex == nullptr) {
        std::cerr << "Error: Geometry or Texture is null!" << std::endl;
        return;
    }

    // Save the current culling state
    GLboolean cullingEnabled = glIsEnabled(GL_CULL_FACE);
    
    glGetIntegerv(GL_CULL_FACE_MODE, &rc.currentCullFace);
    glGetIntegerv(GL_FRONT_FACE, &rc.currentFrontFace);

    // Temporarily disable face culling if cullDisable is true
    if (rc.cullDisable) {
        glDisable(GL_CULL_FACE);
    }

    // Bind the texture
    rc.Tex.get()->Bind();

    // Set the model matrix uniform
    glUniformMatrix4fv(modelMatrixLOC, 1, GL_FALSE, glm::value_ptr(rc.modelMatrix));

    // Draw the geometry
    rc.Geom->Draw(modelMatrixLOC, rc.modelMatrix, colorUniformLOC, false);

    // Unbind the texture
    rc.Tex.get()->Unbind();

    // Restore the original culling state
    if (cullingEnabled) {
        glEnable(GL_CULL_FACE);
        glCullFace(rc.currentCullFace);
        glFrontFace(rc.currentFrontFace);
    }
}

void RenderSystem::tickSystem()
{
    //Draw();
}