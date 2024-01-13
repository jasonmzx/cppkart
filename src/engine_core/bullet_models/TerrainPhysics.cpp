#include "TerrainPhysics.h"
#include "engine_core/singletons/PhysicsWorldSingleton.h"

TerrainPhysics::TerrainPhysics(int width, int length, float* heightData, btScalar minHeight, btScalar maxHeight,
 int absolute_x_offset, int absolute_z_offset, float SCALE_FACTOR) {
        

        // Define the scale - adjust as needed
        btVector3 scale(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);

        //From Docs:
        //* PHY_FLOAT: height at a point is the float value at that grid point. heightScale is ignored when using the float heightfield data type.

        // Create a heightfield terrain shape
        auto* terrainShape = new btHeightfieldTerrainShape(
            width, length, heightData, 1.0, minHeight, maxHeight, 1, PHY_FLOAT, false);

        // Scale the chunk
        terrainShape->setLocalScaling(scale);

        // Define the terrain's position - adjust the position as needed
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(absolute_x_offset*SCALE_FACTOR, 0, absolute_z_offset*SCALE_FACTOR));

        // Create the terrain rigid body
        btScalar mass(0.0); // Zero mass means static
        btVector3 localInertia(0, 0, 0);
        auto* motionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, terrainShape, localInertia);
        
        terrainRigidBody = new btRigidBody(rbInfo);
}
