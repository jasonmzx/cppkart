#include "VehiclePhysics.h"
#include "engine_core/singletons/PhysicsWorldSingleton.h"

VehiclePhysics::VehiclePhysics()
{
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    // Vehicle tuning:
    tuning.m_suspensionStiffness = 20.0f;
    tuning.m_suspensionCompression = 0.83f;
    tuning.m_suspensionDamping = 0.88f;
    tuning.m_maxSuspensionTravelCm = 500.0f;
    tuning.m_frictionSlip = 10.5f;
    tuning.m_maxSuspensionForce = 6000.0f;

    // Vehicle setup
    btBoxShape *vehicleChassisShape = new btBoxShape(btVector3(0.8f, 0.5f, 1.0f));
    btDefaultMotionState *vehicleMotionState = new btDefaultMotionState();
    btTransform localTransform;
    localTransform.setIdentity();
    localTransform.setOrigin(btVector3(0, 3, 0));
    vehicleMotionState->setWorldTransform(localTransform);

    //* VEHICLE MASS !
    btScalar vehicleMass = 750;

    btVector3 vehicleInertia(0, 0, 0);
    vehicleChassisShape->calculateLocalInertia(vehicleMass, vehicleInertia);
    btRigidBody::btRigidBodyConstructionInfo vehicleRigidBodyCI(vehicleMass, vehicleMotionState, vehicleChassisShape, vehicleInertia);

    vehicleRigidBody = new btRigidBody(vehicleRigidBodyCI);
    physicsWorld->dynamicsWorld->addRigidBody(vehicleRigidBody);

    // vehicleRigidBody->getWorldTransform
    // ^ use for rotation camera thingy

    // Raycaster and the actual vehicle
    vehicleRayCaster = new btDefaultVehicleRaycaster(physicsWorld->dynamicsWorld);
    vehicle = new btRaycastVehicle(tuning, vehicleRigidBody, vehicleRayCaster);

    // Wheel configuration
    btVector3 wheelDirection = btVector3(0, -1, 0);
    btVector3 wheelAxle = btVector3(-1, 0, 0);
    btScalar suspensionRestLength = 0.7;
    btScalar wheelRadius = 0.125;
    btScalar wheelWidth = 0.5;
    btScalar suspensionStiffness = 20.0;
    btScalar dampingRelaxation = 2.3;
    btScalar dampingCompression = 2.4;
    btScalar frictionSlip = 1.2;
    btScalar rollInfluence = 0.2;

    //* Adding WHEELS to vehicle physics model !

    auto halfExtents = vehicleChassisShape->getHalfExtentsWithoutMargin();
    btScalar connectionHeight(2);

    btVector3 wheelConnectionPoint(halfExtents.x() - wheelRadius, connectionHeight, halfExtents.z() - wheelWidth);

    // Adds the front wheels
    vehicle->addWheel(wheelConnectionPoint * btVector3(1, 0, 1), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, true);

    vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 0, 1), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, true);

    // Adds the rear wheels
    vehicle->addWheel(wheelConnectionPoint * btVector3(1, 0, -1), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, false);

    vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 0, -1), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, false);

    for (int i = 0; i < vehicle->getNumWheels(); i++)
    {
        btWheelInfo &wheel = vehicle->getWheelInfo(i);
        wheel.m_suspensionStiffness = suspensionStiffness;
         wheel.m_wheelsDampingRelaxation = dampingRelaxation;
         wheel.m_wheelsDampingCompression = dampingCompression;
        //wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); // btScalar(0.8);
        //wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);  // 1;
        
        // Larger friction slips will result in better handling
        wheel.m_frictionSlip = frictionSlip;
        wheel.m_rollInfluence = rollInfluence;
    }

    vehicleRigidBody->setActivationState(DISABLE_DEACTIVATION);
    physicsWorld->dynamicsWorld->addVehicle(vehicle);

    // Vehicle setup:
    engineForce = 0.0;
    vehicleSteering = 0.0;
    steeringIncrement = 0.04;
    steeringClamp = 0.3;
    brakeForce = 0.0;
}

void VehiclePhysics::ApplyEngineForce(float force)
{
    engineForce = force;

    // Rear Wheel Drive

    vehicle->applyEngineForce(engineForce, 2);
    vehicle->applyEngineForce(engineForce, 3);
    // TODO: Add any Bullet physics code here that applies this force
}

void VehiclePhysics::Steer(float value)
{
    //Steer with front wheels
    vehicleSteering = value;
    vehicle->setSteeringValue(value, 0);
    vehicle->setSteeringValue(value, 1);
}

void VehiclePhysics::Brake(float force)
{
    brakeForce = force;
    vehicle->setBrake(brakeForce, 2);
    vehicle->setBrake(brakeForce, 3);

    // TODO: Add Bullet code here to apply brake
}

void VehiclePhysics::Update()
{
    // TODO: Code to update vehicle's state
}

btTransform VehiclePhysics::GetTransform() const
{
    // Returns the current transform (position & rotation) of the vehicle
    return vehicleRigidBody->getWorldTransform();
}

void VehiclePhysics::printState()
{
    btVector3 velocity = vehicleRigidBody->getLinearVelocity();
    btVector3 position = vehicleRigidBody->getWorldTransform().getOrigin();

    printf("Vehicle Velocity (XYZ): %.2f, %.2f, %.2f | Position (XYZ): %.2f, %.2f, %.2f\n",
           velocity.getX(), velocity.getY(), velocity.getZ(),
           position.getX(), position.getY(), position.getZ());

    for (int i = 0; i < vehicle->getNumWheels(); i++)
    {
        btWheelInfo wheelinfo = vehicle->getWheelInfo(i);
        float wX = wheelinfo.m_worldTransform.getOrigin().getX();

        float wY = wheelinfo.m_worldTransform.getOrigin().getY();

        float wZ = wheelinfo.m_worldTransform.getOrigin().getZ();

        printf("%d XYZ: %.2f, %.2f, %.2f \n", i, wX, wY, wZ);
    }
}