#include "StaticTriangleMeshPhysics.hpp"

StaticTriangleMeshPhysics::StaticTriangleMeshPhysics(
    const std::vector<GLfloat> &vertices,
    const std::vector<GLuint> &indices,
    const glm::mat4 &modelMatrix)
{

    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    //* Construction of Mesh (btTriangleMesh is OK for static objects)


    std::unique_ptr<btTriangleMesh> mesh = std::make_unique<btTriangleMesh>();

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        // x y z r g b u v Nx Ny Nz     (11)
        int VERT_LEN = 11;
        GLuint idx0 = indices[i]; GLuint idx1 = indices[i+1]; GLuint idx2 = indices[i+2];
        
        btVector3 vertex0(vertices[idx0 * VERT_LEN], vertices[idx0 * VERT_LEN + 1], vertices[idx0 * VERT_LEN + 2]);
        btVector3 vertex1(vertices[idx1 * VERT_LEN], vertices[idx1 * VERT_LEN + 1], vertices[idx1 * VERT_LEN + 2]);
        btVector3 vertex2(vertices[idx2 * VERT_LEN], vertices[idx2 * VERT_LEN + 1], vertices[idx2 * VERT_LEN + 2]);

        mesh.get()->addTriangle(vertex0, vertex1, vertex2);
    }
    
    std::unique_ptr<btBvhTriangleMeshShape> triangleShape = std::make_unique<btBvhTriangleMeshShape>(mesh.get(), true);

    // Convert glm::mat4 to btTransform, and apply parameters
    
    btTransform staticMeshTransform;
    staticMeshTransform.setFromOpenGLMatrix(glm::value_ptr(modelMatrix));

    staticMeshTransform.setIdentity();
    staticMeshTransform.setOrigin(btVector3(0, 0, 0));

    //! Todo: Fix the hard coded SCALE
    btVector3 scale(40.0f, 40.0f, 40.0f);
    triangleShape.get()->setLocalScaling(scale);

    std::unique_ptr<btDefaultMotionState> motion = std::make_unique<btDefaultMotionState>(staticMeshTransform);

    // Create the rigid body as before
    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion.get(), triangleShape.get());
    info.m_friction = 2.0f;

    meshRigidBody = std::make_unique<btRigidBody>(info);

}