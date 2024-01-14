#ifndef TERRAINPHYSICS_CLASS_H
#define TERRAINPHYSICS_CLASS_H

#include <btBulletDynamicsCommon.h>

#include<stb/stb_image.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include <memory>

class TerrainPhysics {
private:

public:
    std::shared_ptr<btRigidBody> terrainRigidBody;
    TerrainPhysics(int width, int length, float* heightData, btScalar minHeight, btScalar maxHeight, int absolute_x_offset, int absolute_z_offset, float SCALE_FACTOR);
};

#endif