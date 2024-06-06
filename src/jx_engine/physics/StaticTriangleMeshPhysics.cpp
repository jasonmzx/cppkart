#include "StaticTriangleMeshPhysics.hpp"

StaticTriangleMeshPhysics::StaticTriangleMeshPhysics(
    const std::vector<glm::vec3> &ordered_verts,
    const glm::mat4 &modelMatrix)
{

    //* Construction of Mesh (btTriangleMesh is OK for static objects)


    std::unique_ptr<btTriangleMesh> mesh = std::make_unique<btTriangleMesh>();

    for (size_t i = 0; i < ordered_verts.size(); i += 3)
    {
        // x y z r g b u v Nx Ny Nz     (11)
        // int VERT_LEN = 11;
        // GLuint idx0 = indices[i]; GLuint idx1 = indices[i+1]; GLuint idx2 = indices[i+2];
        
        btVector3 vertex0(ordered_verts[i].x, ordered_verts[i].y, ordered_verts[i].z);
        btVector3 vertex1(ordered_verts[i + 1].x, ordered_verts[i + 1].y, ordered_verts[i + 1].z);
        btVector3 vertex2(ordered_verts[i + 2].x, ordered_verts[i + 2].y, ordered_verts[i + 2].z);

        mesh.get()->addTriangle(vertex0, vertex1, vertex2);
    }
    
    std::unique_ptr<btBvhTriangleMeshShape> triangleShape = std::make_unique<btBvhTriangleMeshShape>(mesh.get(), true);

    // Convert glm::mat4 to btTransform, and apply parameters
    
    btTransform staticMeshTransform;
    staticMeshTransform.setFromOpenGLMatrix(glm::value_ptr(modelMatrix));

    staticMeshTransform.setIdentity();
    staticMeshTransform.setOrigin(btVector3(0, 0, 0));

    //! Todo: Fix the hard coded SCALE
    btVector3 scale(5.0f, 5.0f, 5.0f);
    triangleShape.get()->setLocalScaling(scale);

    std::unique_ptr<btDefaultMotionState> motion = std::make_unique<btDefaultMotionState>(staticMeshTransform);

    // Create the rigid body as before
    btRigidBody::btRigidBodyConstructionInfo info(0.0, motion.get(), triangleShape.get());
    info.m_friction = 2.0f;

    meshRigidBody = std::make_unique<btRigidBody>(info);

}