#include "VehiclePhysics.h"
#include "engine_core/singletons/PhysicsWorldSingleton.h"

VehiclePhysics::VehiclePhysics() {
    PhysicsWorldSingleton* physicsWorld = PhysicsWorldSingleton::getInstance();
    
    //Vehicle tuning:
    tuning.m_suspensionStiffness = 20.0f;
    tuning.m_suspensionCompression = 0.83f;
    tuning.m_suspensionDamping = 0.88f;
    tuning.m_maxSuspensionTravelCm = 500.0f;
    tuning.m_frictionSlip = 10.5f;
    tuning.m_maxSuspensionForce = 6000.0f;

// Vehicle setup
    btCollisionShape* vehicleChassisShape = new btBoxShape(btVector3(1.0f, 0.5f, 2.0f));
    btDefaultMotionState* vehicleMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.5, 3, 0.5)));
    btScalar vehicleMass = 800;
    btVector3 vehicleInertia(0, 0, 0);
    vehicleChassisShape->calculateLocalInertia(vehicleMass, vehicleInertia);
    btRigidBody::btRigidBodyConstructionInfo vehicleRigidBodyCI(vehicleMass, vehicleMotionState, vehicleChassisShape, vehicleInertia);
    vehicleRigidBody = new btRigidBody(vehicleRigidBodyCI);
    physicsWorld->dynamicsWorld->addRigidBody(vehicleRigidBody);

    // Raycaster and the actual vehicle
    vehicleRayCaster = new btDefaultVehicleRaycaster(physicsWorld->dynamicsWorld);
    vehicle = new btRaycastVehicle(tuning, vehicleRigidBody, vehicleRayCaster);
    vehicleRigidBody->setActivationState(DISABLE_DEACTIVATION);
    physicsWorld->dynamicsWorld->addVehicle(vehicle);

// Wheel configuration
    btVector3 wheelDirection = btVector3(0, -1, 0);
    btVector3 wheelAxle = btVector3(-1, 0, 0);
    btScalar suspensionRestLength = 0.6;
    btScalar wheelRadius = 0.5;
    btScalar wheelWidth = 0.4;
    btScalar suspensionStiffness = 20.0;
    btScalar dampingRelaxation = 2.3;
    btScalar dampingCompression = 4.4;
    btScalar frictionSlip = 1000;
    btScalar rollInfluence = 0.1;

//TODO: Add wheels to the vehicle LOOK OVER THIS
for (int i = 0; i < 4; i++)
{
    bool isFrontWheel = i < 2;
    vehicle->addWheel(
        btVector3(0, 1.6, i == 0 || i == 3 ? 1.2 : -1.2),
        wheelDirection,
        wheelAxle,
        suspensionRestLength,
        wheelRadius,
        tuning,
        isFrontWheel);
    btWheelInfo& wheel = vehicle->getWheelInfo(i);
    wheel.m_suspensionStiffness = suspensionStiffness;
    wheel.m_wheelsDampingRelaxation = dampingRelaxation;
    wheel.m_wheelsDampingCompression = dampingCompression;
    wheel.m_frictionSlip = frictionSlip;
    wheel.m_rollInfluence = rollInfluence;
}

vehicleRigidBody->setActivationState(DISABLE_DEACTIVATION);
    
    
    //Vehicle setup:
    engineForce = 0.0;
    vehicleSteering = 0.0;
    steeringIncrement = 0.04;
    steeringClamp = 0.3;
    brakeForce = 0.0;


}

void VehiclePhysics::ApplyEngineForce(float force) {
    engineForce = force;

    //Rear Wheel Drive?

    vehicle->applyEngineForce(engineForce, 2);
    vehicle->applyEngineForce(engineForce, 3);
    //TODO: Add any Bullet physics code here that applies this force
}

void VehiclePhysics::Steer(float value) {
    vehicleSteering = value;
    //TODO: Add Bullet code here to apply this steering value
}

void VehiclePhysics::Brake(float force) {
    brakeForce = force;
    vehicle->setBrake(brakeForce, 2);
    vehicle->setBrake(brakeForce, 3);
    
    //TODO: Add Bullet code here to apply brake
}

void VehiclePhysics::Update() {
    //TODO: Code to update vehicle's state
}

btTransform VehiclePhysics::GetTransform() const {
    // Returns the current transform (position & rotation) of the vehicle
    return vehicleRigidBody->getWorldTransform();
}

void VehiclePhysics::printState() {
   btVector3 velocity = vehicleRigidBody->getLinearVelocity();
   btVector3 position = vehicleRigidBody->getWorldTransform().getOrigin();

    printf("Vehicle Velocity (XYZ): %.2f, %.2f, %.2f | Position (XYZ): %.2f, %.2f, %.2f\n",
           velocity.getX(), velocity.getY(), velocity.getZ(),
           position.getX(), position.getY(), position.getZ());
  
}