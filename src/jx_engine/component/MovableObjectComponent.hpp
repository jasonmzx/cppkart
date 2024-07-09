#ifndef MOVABLEOBJECTCOMPONENT_HPP
#define MOVABLEOBJECTCOMPONENT_HPP

#include "RenderComponent.hpp"
#include "jx_engine/physics/DynamicConvexHullMeshPhysics.hpp"

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class MovableObjectComponent : public RenderComponent 
{
public:
    MovableObjectComponent(std::string modelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm, int meshIndex, bool cD, bool isTexAlpha, float scale, float mass);
    
    void UpdateTransforms();

    DynamicConvexHullMeshPhysics* phyMesh;
    btRigidBody* GetRigidBody();
};

#endif // MOVABLEOBJECTCOMPONENT_HPP
