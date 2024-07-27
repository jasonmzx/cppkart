#include "PhysicsWorldSingleton.h"

PhysicsWorldSingleton* PhysicsWorldSingleton::instance = nullptr;

PhysicsWorldSingleton::PhysicsWorldSingleton() {
    
    broadphase = new btDbvtBroadphase(); //uses QuadTree Data Structure to minimize expensive collision checking

    //broadphase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000)); // TODO: Make sure world size is correct

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher); // For Triangle on Triangle Collision (GImpact Mesh Shape)

    solver = new btSequentialImpulseConstraintSolver();
    
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0f, -17.81f , 0.0f));

}

PhysicsWorldSingleton* PhysicsWorldSingleton::getInstance() {
    if (!instance)
        instance = new PhysicsWorldSingleton;
    return instance;
}
