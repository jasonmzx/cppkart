#include "StaticTriangleMeshPhysics.hpp"

StaticTriangleMeshPhysics::StaticTriangleMeshPhysics(
    const std::vector<glm::vec3> &ordered_verts, const glm::mat4 &modelMatrix, float scaleFactor)
{

    //* Construction of Mesh (btTriangleMesh is OK for static objects)

    mesh = new btTriangleMesh();

    for (size_t i = 0; i < ordered_verts.size(); i += 3)
    {
        btVector3 vertex0(ordered_verts[i    ].x, ordered_verts[i    ].y, ordered_verts[i    ].z);
        btVector3 vertex1(ordered_verts[i + 1].x, ordered_verts[i + 1].y, ordered_verts[i + 1].z);
        btVector3 vertex2(ordered_verts[i + 2].x, ordered_verts[i + 2].y, ordered_verts[i + 2].z);

        mesh->addTriangle(vertex0, vertex1, vertex2);
    }
    
    triangleShape = new btBvhTriangleMeshShape(mesh, true);

    // Convert glm::mat4 to btTransform, and apply parameters
    
    btTransform staticMeshTransform;
    staticMeshTransform.setFromOpenGLMatrix(glm::value_ptr(modelMatrix));

    staticMeshTransform.setIdentity();
    //staticMeshTransform.setOrigin(btVector3(0, 0, 0));

    // Scale of Mesh (XYZ Scale)
    btVector3 scale(scaleFactor, scaleFactor, scaleFactor);
    triangleShape->setLocalScaling(scale);

    //std::unique_ptr<btDefaultMotionState> motion = std::make_unique<btDefaultMotionState>(staticMeshTransform);
    btDefaultMotionState* motion = new btDefaultMotionState(staticMeshTransform);

    // Create the rigid body as before
    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, triangleShape, btVector3(0, 0, 0));
    info.m_friction = 3.0f; //TODO: Add friction parameter

    meshRigidBody = new btRigidBody(info);

}

StaticTriangleMeshPhysics::~StaticTriangleMeshPhysics()
{
    delete meshRigidBody->getMotionState();
    delete meshRigidBody;
    delete triangleShape;
    delete mesh;
}