#ifndef PHYSICSWORLDSINGLETON_CLASS_H
#define PHYSICSWORLDSINGLETON_CLASS_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

class PhysicsWorldSingleton {
private:
    PhysicsWorldSingleton();
    static PhysicsWorldSingleton* instance;

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;

public:
    btDiscreteDynamicsWorld* dynamicsWorld;

    static PhysicsWorldSingleton* getInstance();
};

#endif