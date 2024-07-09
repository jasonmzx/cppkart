#ifndef PHYSICSWORLDSINGLETON_CLASS_H
#define PHYSICSWORLDSINGLETON_CLASS_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

enum CollisionGroups {
    COLLISION_GROUP_NONE = 0,         // 0000  
    COLLISION_GROUP_VEHICLE = 1 << 0, // 0001
    COLLISION_GROUP_BARRIER = 1 << 1, // 0010
    COLLISION_GROUP_CHUNKS = 1 << 2,  // 0100
    COLLISION_GROUP_ALL = COLLISION_GROUP_VEHICLE | COLLISION_GROUP_BARRIER | COLLISION_GROUP_CHUNKS // 0111
};

class PhysicsWorldSingleton {
private:
    PhysicsWorldSingleton();
    static PhysicsWorldSingleton* instance;

    btBroadphaseInterface* broadphase; //Broad Phase when checking for Collisions

    btDefaultCollisionConfiguration* collisionConfiguration; // Coupled with "dispatcher", the config is default for now...

    btCollisionDispatcher* dispatcher; //Setting a Collision Algorithm 

    btSequentialImpulseConstraintSolver* solver;

public:
    btDiscreteDynamicsWorld* dynamicsWorld;

    static PhysicsWorldSingleton* getInstance();
};

#endif