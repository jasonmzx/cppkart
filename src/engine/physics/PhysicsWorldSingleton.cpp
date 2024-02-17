#include "PhysicsWorldSingleton.h"

PhysicsWorldSingleton* PhysicsWorldSingleton::instance = nullptr;

PhysicsWorldSingleton::PhysicsWorldSingleton() {
    
    broadphase = new btDbvtBroadphase(); //uses QuadTree Data Structure to minimize expensive collision checking

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver();
    
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0f, -17.81f , 0.0f));

}

PhysicsWorldSingleton* PhysicsWorldSingleton::getInstance() {
    if (!instance)
        instance = new PhysicsWorldSingleton;
    return instance;
}
