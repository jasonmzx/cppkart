#include "PhysicsWorldSingleton.h"

PhysicsWorldSingleton* PhysicsWorldSingleton::instance = nullptr;

PhysicsWorldSingleton::PhysicsWorldSingleton() {
    
    broadphase = new btDbvtBroadphase(); //uses QuadTree Data Structure to minimize expensive collision checking

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver();
    
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0f, -13.81f, 0.0f));

}

PhysicsWorldSingleton* PhysicsWorldSingleton::getInstance() {
    if (!instance)
        instance = new PhysicsWorldSingleton;
    return instance;
}

void PhysicsWorldSingleton::Destroy() {
    if (instance) {

        if (instance->dynamicsWorld) {
            delete instance->dynamicsWorld;
            instance->dynamicsWorld = nullptr;
        }

        if (instance->solver) {
            delete instance->solver;
            instance->solver = nullptr;
        }

        if (instance->dispatcher) {
            delete instance->dispatcher;
            instance->dispatcher = nullptr;
        }

        if (instance->collisionConfiguration) {
            delete instance->collisionConfiguration;
            instance->collisionConfiguration = nullptr;
        }

        if (instance->broadphase) {
            delete instance->broadphase;
            instance->broadphase = nullptr;
        }

        // Finally, delete the instance itself
        delete instance;
        instance = nullptr;
    }
}