#include "RenderCylinderComponent.hpp"

RenderCylinderComponent::RenderCylinderComponent()
{

    GameGLRenderer* renderer = GameGLRenderer::getInstance();

    useTextureLOC = renderer->useTextureLOC;
    modelMatrixLOC = renderer->modelMatrixLOC;
    colorUniformLOC = renderer->colorUniformLocation;


    std::vector<GLfloat> vertices = renderer->GetConeVertices(60, 5);
    std::vector<GLuint> indices = renderer->GetConeIndices(60);


    std::shared_ptr<Geometry> geom_construct = std::make_shared<Geometry>(vertices, indices);

    this->Geom = geom_construct;

    float scale = 1.0f;

    ObjmodelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));

}

void RenderCylinderComponent::Draw()
{

    glUniform1i(useTextureLOC, GL_FALSE);

    // Save the current culling state
    GLboolean cullingEnabled = glIsEnabled(GL_CULL_FACE);
    
    glGetIntegerv(GL_CULL_FACE_MODE, &currentCullFace);
    glGetIntegerv(GL_FRONT_FACE, &currentFrontFace);

    glDisable(GL_CULL_FACE);

    Geom->Draw(modelMatrixLOC, ObjmodelMatrix, colorUniformLOC, false);

    glEnable(GL_CULL_FACE);

    glUniform1i(useTextureLOC, GL_TRUE);
}

RenderCylinderComponent::~RenderCylinderComponent()
{
}

void RenderCylinderComponent::tick()
{
    Draw();
}