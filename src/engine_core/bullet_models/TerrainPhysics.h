#ifndef TERRAINPHYSICS_CLASS_H
#define TERRAINPHYSICS_CLASS_H

#include <btBulletDynamicsCommon.h>

#include<stb/stb_image.h>
#include <iostream>
#include <vector>
#include <glad/glad.h>

class TerrainPhysics {
private:
    btRigidBody* terrainRigidBody;

public:
    TerrainPhysics(int width, int length, unsigned short* heightData, btScalar minHeight, btScalar maxHeight);
};

#endif