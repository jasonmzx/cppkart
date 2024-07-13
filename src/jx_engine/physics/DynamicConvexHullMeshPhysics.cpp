#include "DynamicConvexHullMeshPhysics.hpp"

DynamicConvexHullMeshPhysics::DynamicConvexHullMeshPhysics(const std::vector<glm::vec3> &ordered_verts, const glm::mat4 &modelMatrix, float scaleFactor, float mass) 
{

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    // Create a triangle mesh
    triangleMesh = new btTriangleMesh();

    for (size_t i = 0; i < ordered_verts.size(); i += 3) {
        const glm::vec3 &v0 = ordered_verts[i];
        const glm::vec3 &v1 = ordered_verts[i + 1];
        const glm::vec3 &v2 = ordered_verts[i + 2];
        
        triangleMesh->addTriangle(btVector3(v0.x, v0.y, v0.z),
                                  btVector3(v1.x, v1.y, v1.z),
                                  btVector3(v2.x, v2.y, v2.z), false);
    }

    // Create the GImpact mesh shape
    auto gimpactShape = new btGImpactMeshShape(triangleMesh);
    gimpactShape->updateBound();

    // Create the motion state
    btTransform transform;
    transform.setIdentity();
    transform.setFromOpenGLMatrix(glm::value_ptr(modelMatrix));

    // // Apply scaling
    // btVector3 scale(scaleFactor, scaleFactor, scaleFactor);
    // gimpactShape->setLocalScaling(scale);

    motionState = new btDefaultMotionState(transform);

    // Create the rigid body
    btVector3 inertia(0, 0, 0);
    gimpactShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, gimpactShape, inertia);
    meshRigidBody = new btRigidBody(rigidBodyCI);
    //meshRigidBody->setUserPointer(this);

    meshRigidBody->setUserPointer((void *)7);
    meshRigidBody->setCollisionFlags(meshRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    // Set the activation state : 1 = active, 0 = inactive
    //meshRigidBody->setActivationState(DISABLE_DEACTIVATION);

    physicsWorld->dynamicsWorld->addRigidBody(meshRigidBody, COLLISION_GROUP_ALL, COLLISION_GROUP_CHUNKS);


}

btTransform DynamicConvexHullMeshPhysics::GetTransform() const
{
    btTransform trans;
    motionState->getWorldTransform(trans);
    return trans;
}

void DynamicConvexHullMeshPhysics::SetPosition(float x, float y, float z)
{
    btTransform transform;
    motionState->getWorldTransform(transform);
    transform.setOrigin(btVector3(x, y, z));
    motionState->setWorldTransform(transform);
    meshRigidBody->setWorldTransform(transform);

    meshRigidBody->setLinearVelocity(btVector3(0, 0, 0));
    //meshRigidBody->setAngularVelocity(btVector3(0, 0, 0));
    //meshRigidBody->clearForces();
}

#if 0
    delete meshRigidBody->getMotionState();
    delete meshRigidBody;
    delete triangleShape;
    delete mesh;
#endif

DynamicConvexHullMeshPhysics::~DynamicConvexHullMeshPhysics() 
{
    delete meshRigidBody->getMotionState();
    delete meshRigidBody->getCollisionShape();
    delete meshRigidBody;
    delete triangleMesh;
}
