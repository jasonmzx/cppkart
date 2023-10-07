#include "PhysicsWorldSingleton.h"

PhysicsWorldSingleton* PhysicsWorldSingleton::instance = nullptr;

PhysicsWorldSingleton::PhysicsWorldSingleton() {
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
}

PhysicsWorldSingleton* PhysicsWorldSingleton::getInstance() {
    if (!instance)
        instance = new PhysicsWorldSingleton;
    return instance;
}