#ifndef TERRAINPHYSICS_CLASS_H
#define TERRAINPHYSICS_CLASS_H

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include<stb/stb_image.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>

//Memory Management Imports
#include <memory>

//* This class keeps Allocated Float Array & Bullet Classes as Shared Pointers for Dynamic De-Allocation & Memory leak Mitigation

class TerrainPhysics {
private:

    std::shared_ptr<float[]> heightData; 
    std::shared_ptr<btHeightfieldTerrainShape> terrainShape;
    std::shared_ptr<btDefaultMotionState> motionState; 

public:

    std::shared_ptr<btRigidBody> terrainRigidBody;
    TerrainPhysics(int width, int length, std::shared_ptr<float[]> heightData, btScalar minHeight, btScalar maxHeight, int absolute_x_offset, int absolute_z_offset, float SCALE_FACTOR);
};

#endif