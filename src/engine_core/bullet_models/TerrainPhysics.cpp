#include "TerrainPhysics.h"
#include "engine_core/singletons/PhysicsWorldSingleton.h"

TerrainPhysics::TerrainPhysics(int w, int l, unsigned short* data, btScalar minH, btScalar maxH)
    : width(w), length(l), heightData(data), minHeight(minH), maxHeight(maxH) {
    
    heightfieldShape = new btHeightfieldTerrainShape(
        width, 
        length, 
        heightData,
        1.0f,  // height scale
        minHeight,
        maxHeight,
        2,     // upAxis = Z
        PHY_SHORT, //Scalar Type (SHORT)
        false  // flipQuadEdges
    );

    btTransform localTransform;
    localTransform.setIdentity();
    localTransform.setOrigin(btVector3(-width/2, (maxHeight-minHeight)/2, -length/2));
    heightfieldShape->setLocalScaling(btVector3(1, 1, 1));  // Assuming no scaling

    btDefaultMotionState* terrainMotionState = new btDefaultMotionState(localTransform);
    btRigidBody::btRigidBodyConstructionInfo terrainRigidBodyCI(0, terrainMotionState, heightfieldShape, btVector3(0, 0, 0));
    terrainRigidBody = new btRigidBody(terrainRigidBodyCI);
}

TerrainPhysics::~TerrainPhysics() {
    delete terrainRigidBody->getMotionState();
    delete terrainRigidBody;
    delete heightfieldShape;
}

btRigidBody* TerrainPhysics::GetRigidBody() const {
    return terrainRigidBody;
}