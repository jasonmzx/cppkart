#ifndef DYNAMIC_CONVEX_HULL_MESHPHYSICS_CLASS_HPP
#define DYNAMIC_CONVEX_HULL_MESHPHYSICS_CLASS_HPP

#include <bullet/btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>

class DynamicConvexHullMeshPhysics {
private:

public:
    btRigidBody* meshRigidBody;

    btDefaultMotionState* motionState;

    btTransform GetTransform() const;

    DynamicConvexHullMeshPhysics(const std::vector<glm::vec3> &ordered_verts, const glm::mat4 &modelMatrix, float scaleFactor, float mass);
    ~DynamicConvexHullMeshPhysics();
};

#endif // DYNAMIC_CONVEX_HULL_MESHPHYSICS_CLASS_HPP
