#ifndef PHYSICSWORLDSINGLETON_CLASS_H
#define PHYSICSWORLDSINGLETON_CLASS_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

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