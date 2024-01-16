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

class TerrainPhysics {
private:

    std::shared_ptr<float[]> heightData; //This class keeps the Height Data Float Ptr Arr Ref, that's dynamically freed with shared-ptr
    btHeightfieldTerrainShape* terrainShape; // Add as member variable
    btDefaultMotionState* motionState; 

public:
    std::shared_ptr<btRigidBody> terrainRigidBody;
    TerrainPhysics(int width, int length, std::shared_ptr<float[]> heightData, btScalar minHeight, btScalar maxHeight, int absolute_x_offset, int absolute_z_offset, float SCALE_FACTOR);
};

#endif