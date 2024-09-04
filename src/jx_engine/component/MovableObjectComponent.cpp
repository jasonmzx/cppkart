#include "MovableObjectComponent.hpp"

MovableObjectComponent::MovableObjectComponent(
    std::string modelPath, std::string texPath, std::vector<int> meshIndices, bool cD, bool isTexAlpha, float scale, float mass) 
: RenderComponent(modelPath, texPath, meshIndices, cD, isTexAlpha)
{

    std::vector<glm::vec3> xyz_verts = this->GetXYZvertices();
    
    //TODO include scaling on mdl matrix so there's no need to scale the physics mesh

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::mat4 scaledMdlMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));

    DynamicConvexHullMeshPhysics *roadBarrierPhysicsComponent = new DynamicConvexHullMeshPhysics(xyz_verts, scaledMdlMatrix, scale, mass);

    phyMesh.reset(roadBarrierPhysicsComponent);

}

void MovableObjectComponent::UpdateTransforms()
{
    // Update the model matrix
    btTransform tr = phyMesh->GetTransform();
    tr.getOpenGLMatrix(glm::value_ptr(ObjmodelMatrix));
}

btRigidBody* MovableObjectComponent::GetRigidBody()
{
    return phyMesh->meshRigidBody;
}

void MovableObjectComponent::SetPosition(float x, float y, float z)
{
    phyMesh->SetPosition(x, y, z);
    this->UpdateTransforms();
}

void MovableObjectComponent::tick()
{
    // this->UpdateTransforms();
    this->Draw();
}