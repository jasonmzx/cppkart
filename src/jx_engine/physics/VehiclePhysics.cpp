#include "VehiclePhysics.hpp"

#include "bullet/btFilterableVehicleRaycaster.h"

VehiclePhysics::VehiclePhysics(float xPos, float yPos, float zPos)
{
    PhysicsWorldSingleton *physicsWorld = PhysicsWorldSingleton::getInstance();

    VEHICLE_SCALE = 0.9;

    // Vehicle tuning:
    tuning.m_suspensionStiffness = 90.0f;
    tuning.m_suspensionCompression = 0.83f;
    tuning.m_suspensionDamping = 0.88f;
    tuning.m_maxSuspensionTravelCm = 100.0f;
    tuning.m_frictionSlip = 10.5f;
    tuning.m_maxSuspensionForce = 6000.0f;

    // Vehicle setup
    
    //Vehicle Box Shape:
    btBoxShape *vehicleChassisShape = new btBoxShape(btVector3(1.6f * VEHICLE_SCALE, 0.5f * VEHICLE_SCALE, 3.0f * VEHICLE_SCALE));
    
    // float capsuleRadius = 1.0f * VEHICLE_SCALE;
    // float capsuleHeight = 6.0f * VEHICLE_SCALE; // Adjusted height to maintain overall length
    // btCapsuleShapeZ *vehicleChassisShape = new btCapsuleShapeZ(capsuleRadius, capsuleHeight);

    vehicleMotionState = new btDefaultMotionState();
    btTransform localTransform;

    localTransform.setIdentity();
    localTransform.setOrigin(btVector3(xPos, yPos, zPos));

    btQuaternion initialRotation( btVector3(0,1,0), 90.0f * SIMD_PI / 180.0f ); // Rotate 90 degrees arround Y axis
    localTransform.setRotation(initialRotation);    

    vehicleMotionState->setWorldTransform(localTransform);

    //* VEHICLE MASS !
    btScalar vehicleMass = 600;

    btVector3 vehicleInertia(0, 0, 0);
    vehicleChassisShape->calculateLocalInertia(vehicleMass, vehicleInertia);
    btRigidBody::btRigidBodyConstructionInfo vehicleRigidBodyCI(vehicleMass, vehicleMotionState, vehicleChassisShape, vehicleInertia);

    vehicleRigidBody = new btRigidBody(vehicleRigidBodyCI);
    vehicleRigidBody->setUserPointer((void *)1);
    vehicleRigidBody->setCollisionFlags(vehicleRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    physicsWorld->dynamicsWorld->addRigidBody(vehicleRigidBody, COLLISION_GROUP_ALL, COLLISION_GROUP_CHUNKS);

    // vehicleRigidBody->getWorldTransform
    // ^ use for rotation camera thingy


    // Raycaster and the actual vehicle
    vehicleRayCaster = new btFilterableVehicleRaycaster(physicsWorld->dynamicsWorld);
    
    vehicleRayCaster->setCollisionFilterGroup(COLLISION_GROUP_ALL);
    vehicleRayCaster->setCollisionFilterMask(COLLISION_GROUP_CHUNKS);

    vehicle = new btRaycastVehicle(tuning, vehicleRigidBody, vehicleRayCaster);

    btVector3 wheelDirection = btVector3(0, -1, 0);
    btVector3 wheelAxle = btVector3(-1.0, 0, 0); //used to be -1
    
    btScalar suspensionRestLength = 0.75;
    btScalar wheelRadius = 1.5*VEHICLE_SCALE;
    btScalar wheelWidth = 0.6;
    btScalar suspensionStiffness = 40.0;
    btScalar dampingRelaxation = 4.3;
    btScalar dampingCompression = 2.4;
    btScalar frictionSlip = 10;
    btScalar rollInfluence = 0.1;

    //* Adding WHEELS to vehicle physics model !

    //!This like, fixes the wonky vehicle sim lmao
    vehicle->setCoordinateSystem(0, 1, 2);

    //auto halfExtents = vehicleChassisShape->getHalfExtentsWithoutMargin();

    //printf("[vp] : Half Extents: %.2f, %.2f, %.2f\n", halfExtents.x(), halfExtents.y(), halfExtents.z());


    btScalar connectionHeight = 3.2f;

    //btVector3 wheelConnectionPoint(halfExtents.x() - 0.6, connectionHeight, halfExtents.z() - 0.5);

    btVector3 wheelConnectionPoint = btVector3(1.40 - 0.6, connectionHeight, 2.66 - 0.5);

    // Adds the front wheels
    vehicle->addWheel(wheelConnectionPoint * btVector3(2, 0, 1.2), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, true);

    vehicle->addWheel(wheelConnectionPoint * btVector3(-2, 0, 1.2), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, true);

    // Adds the rear wheels
    vehicle->addWheel(wheelConnectionPoint * btVector3(2, 0, -1.1), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, false);

    vehicle->addWheel(wheelConnectionPoint * btVector3(-2, 0, -1.1), wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, tuning, false);

    for (int i = 0; i < vehicle->getNumWheels(); i++)
    {
        btWheelInfo &wheel = vehicle->getWheelInfo(i);
        
        // Larger 
        //wheel.m_chassisConnectionPointCS = wheelConnectionPoint;
        
         wheel.m_suspensionStiffness = suspensionStiffness;
         wheel.m_wheelsDampingRelaxation = dampingRelaxation;
         
        // wheel.m_wheelsDampingCompression = dampingCompression;
         wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness); // btScalar(0.8);
         wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);  // 1;
        
        // Larger friction slips will result in better handling
        wheel.m_frictionSlip = frictionSlip;
        wheel.m_rollInfluence = rollInfluence;

    }

    vehicleRigidBody->setActivationState(DISABLE_DEACTIVATION);

//! Experimental
// vehicleRigidBody->setLinearVelocity(btVector3(0, 0, 0));
// vehicleRigidBody->setAngularVelocity(btVector3(0, 0, 0));

    physicsWorld->dynamicsWorld->addVehicle(vehicle);

    // Vehicle setup:
    engineForce = 0.0;
    brakeForce = 0.0;

    vehicleSteering = 0.0;
    steeringIncrement = 0.04;

    steeringClamp = 0.35;
    MaxSteeringClamp = 0.35;

}

void VehiclePhysics::ApplyEngineForce(float force)
{
    engineForce = force;

    // Rear Wheel Drive

    vehicle->applyEngineForce(engineForce, 0);
    vehicle->applyEngineForce(engineForce, 1);
    // TODO: Add any Bullet physics code here that applies this force
}



float VehiclePhysics::clampSteerAngleAtSpeed(float speed) {
    // Define the parameters for the steering angle interpolation
    float maxSteer = 0.35f;  // Max steer angle at low speed
    float midSteer = 0.05f;  // Steer angle at mid speed (150.0f)
    float minSteer = 0.01f;  // Min steer angle at high speed
    float midSpeed = 150.0f; // Speed at which steer angle is 0.05f
    float maxSpeed = 250.0f; // Max speed for interpolation

    // Clamp the speed to the range [0, maxSpeed]
    if (speed > maxSpeed) {
        speed = maxSpeed;
    } else if (speed < 0.0f) {
        speed = 0.0f;
    }

    float steerAngle;

    if (speed <= midSpeed) {
        // Interpolate between maxSteer and midSteer
        steerAngle = maxSteer + (midSteer - maxSteer) * (speed / midSpeed);
    } else {
        // Interpolate between midSteer and minSteer
        steerAngle = midSteer + (minSteer - midSteer) * ((speed - midSpeed) / (maxSpeed - midSpeed));
    }

    return steerAngle;
}




void VehiclePhysics::ApplySteer(float steerIncr)
{
    if (steerIncr != 0)
    {

        // Get the linear velocity of the vehicle's rigid body
        float speed = getSpeed();
        steeringClamp = clampSteerAngleAtSpeed(speed);

        printf("STEER CLAMP: %.2f\n", steeringClamp);

        // Increment or decrement steering based on steerIncr
        vehicleSteering += steerIncr;

        // Clamp the steering value to make sure it's within [-steeringClamp, steeringClamp]
        if (vehicleSteering > steeringClamp) {
            vehicleSteering = steeringClamp;
        } else if (vehicleSteering < -steeringClamp) {
            vehicleSteering = -steeringClamp;
        }
    }
    else
    {
        // Tend the steering value back to zero if steerIncr is 0
        if (vehicleSteering > steeringIncrement) {
            vehicleSteering -= steeringIncrement;
        } else if (vehicleSteering < -steeringIncrement) {
            vehicleSteering += steeringIncrement;
        } else {
            vehicleSteering = 0; // Close enough to zero we can set it directly
        }
    }

    // Apply the steering value to the front wheels
    vehicle->setSteeringValue(vehicleSteering, 0);
    vehicle->setSteeringValue(vehicleSteering, 1);
}

float VehiclePhysics::getSpeed() const
{
    // Get the linear velocity of the vehicle's rigid body
    btVector3 velocity = vehicleRigidBody->getLinearVelocity();
    // Calculate the magnitude (length) of the velocity vector
    return velocity.length();
}

void VehiclePhysics::Brake(float force)
{
    brakeForce = force;
    vehicle->setBrake(brakeForce, 2);
    vehicle->setBrake(brakeForce, 3);

}


void VehiclePhysics::ResetTransform()
{
    btTransform newTransform;
    newTransform.setIdentity();

    // Get the current position and adjust the Y position
    btVector3 currentVehiclePos = vehicleRigidBody->getWorldTransform().getOrigin();
    currentVehiclePos.setY(currentVehiclePos.getY() + 10.0f);

    // Set the new position
    newTransform.setOrigin(currentVehiclePos);

    // Set the world transform for both the motion state and the rigid body
    vehicleMotionState->setWorldTransform(newTransform);
    vehicleRigidBody->setWorldTransform(newTransform);

    // Clear the vehicle's velocities
    vehicleRigidBody->setLinearVelocity(btVector3(0, 0, 0));
    vehicleRigidBody->setAngularVelocity(btVector3(0, 0, 0));

    // Re-activate the vehicle
    vehicleRigidBody->activate(true);
}



btTransform VehiclePhysics::GetTransform() const
{
    btTransform trans;

    // Ensure the motion state and rigid body are synchronized
    vehicleMotionState->getWorldTransform(trans);
    vehicleRigidBody->getMotionState()->getWorldTransform(trans);

    return trans;
}

std::string VehiclePhysics::debugStateSTR() 
{
    btVector3 velocity = vehicleRigidBody->getLinearVelocity();
    btVector3 vehiclePosition = vehicleRigidBody->getWorldTransform().getOrigin();

    char debugVehicle_STR[1024];
    sprintf(debugVehicle_STR,
            "(X,Y,Z)\n\n"
            "POS=      %.2f , %.2f , %.2f\n\n"
            "VELOCITY= %.2f , %.2f , %.2f\n\n"
            "STEER=    %.2f , %.2f",
            vehiclePosition.getX(), vehiclePosition.getY(), vehiclePosition.getZ(),
            velocity.getX(), velocity.getY(), velocity.getZ(),
            vehicle->getSteeringValue(0), vehicle->getSteeringValue(1));

    return std::string(debugVehicle_STR);
}

float VehiclePhysics::getX() const
{
    return vehicleRigidBody->getWorldTransform().getOrigin().getX();
}

float VehiclePhysics::getY() const
{
    return vehicleRigidBody->getWorldTransform().getOrigin().getY();
}

float VehiclePhysics::getZ() const
{
    return vehicleRigidBody->getWorldTransform().getOrigin().getZ();
}
