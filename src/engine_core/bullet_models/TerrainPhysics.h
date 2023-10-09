#ifndef TERRAINPHYSICS_CLASS_H
#define TERRAINPHYSICS_CLASS_H

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

class TerrainPhysics {
private:
    btHeightfieldTerrainShape* heightfieldShape;
    btRigidBody* terrainRigidBody;

    int width;
    int length;
    unsigned short* heightData;  // Assuming the height data is of type unsigned short
    btScalar maxHeight;
    btScalar minHeight;

public:
    TerrainPhysics(int width, int length, unsigned short* heightData, btScalar minHeight, btScalar maxHeight);
    ~TerrainPhysics();

    btRigidBody* GetRigidBody() const;
};

#endif