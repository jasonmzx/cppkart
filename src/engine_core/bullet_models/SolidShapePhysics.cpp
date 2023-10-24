#include "SolidShapePhysics.h"
#include "engine_core/singletons/PhysicsWorldSingleton.h"


SolidShapePhysics::SolidShapePhysics(
        const std::vector<GLfloat> &vertices,
        const std::vector<GLuint> &indices,
        const glm::mat4 &modelMatrix)
{

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    // For simplicity, let's use a triangle mesh for the collision shape.
    // This is fine for static objects but not recommended for dynamic ones.
    btTriangleMesh *mesh = new btTriangleMesh();
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        btVector3 vertex0(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
        btVector3 vertex1(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]);
        btVector3 vertex2(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]);

        mesh->addTriangle(vertex0, vertex1, vertex2);
    }

    collisionShape = new btBvhTriangleMeshShape(mesh, true);

    // Convert the glm model matrix to Bullet transform
    btTransform transform;
    transform.setFromOpenGLMatrix(glm::value_ptr(modelMatrix));

    motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, motionState, collisionShape);
    rigidBody = new btRigidBody(constructionInfo);

    physicsWorld->dynamicsWorld->addRigidBody(rigidBody);
    printf("ADDED RIGID BODY...");
}

SolidShapePhysics::~SolidShapePhysics()
{
    delete rigidBody;
    delete motionState;
    delete collisionShape;
}

btRigidBody *SolidShapePhysics::getRigidBody() const
{
    return rigidBody;
}