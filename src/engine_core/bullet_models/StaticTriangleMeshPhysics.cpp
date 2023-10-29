#include "StaticTriangleMeshPhysics.h"

StaticTriangleMeshPhysics::StaticTriangleMeshPhysics(
    const std::vector<GLfloat> &vertices,
    const std::vector<GLuint> &indices,
    const glm::mat4 &modelMatrix)
{

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    //* Construction of Mesh (btTriangleMesh is OK for static objects)

    btTriangleMesh *mesh = new btTriangleMesh();
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        btVector3 vertex0(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
        btVector3 vertex1(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]);
        btVector3 vertex2(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]);

        mesh->addTriangle(vertex0, vertex1, vertex2);
    }

    btBvhTriangleMeshShape* triangleShape = new btBvhTriangleMeshShape(mesh, true);

    // Assuming you have the same transformation and motion state:
    btTransform staticMeshTransform;
    staticMeshTransform.setIdentity();
    staticMeshTransform.setOrigin(btVector3(0, 0, 0));

    btMotionState *motion = new btDefaultMotionState(staticMeshTransform);

    // Create the rigid body as before
    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, triangleShape);
    info.m_friction = 2.0f;

    btRigidBody *staticTriRigidBody = new btRigidBody(info);
    physicsWorld->dynamicsWorld->addRigidBody(staticTriRigidBody);
}