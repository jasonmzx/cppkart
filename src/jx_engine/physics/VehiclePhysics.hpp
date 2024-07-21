#ifndef VEHICLEPHYSICS_CLASS_HPP
#define VEHICLEPHYSICS_CLASS_HPP

#include <bullet/btBulletDynamicsCommon.h>  // Assuming you have this included for Bullet physics types
#include "bullet/btFilterableVehicleRaycaster.h"
#include "PhysicsWorldSingleton.h"

class VehiclePhysics {
private:
    btRigidBody* vehicleRigidBody;
    btFilterableVehicleRaycaster* vehicleRayCaster;

    btDefaultMotionState *vehicleMotionState;

    btRaycastVehicle::btVehicleTuning tuning;

    btScalar VEHICLE_SCALE;



    float engineForce;
    float brakeForce;


    float vehicleSteering;
    float steeringIncrement;
    float steeringClamp;

    float MaxSteeringClamp;

    //Will make this slowly incr/decr for steer inputs
    float clampSteerAngleAtSpeed(float speed);


public:
    VehiclePhysics(float xPos, float yPos, float zPos);
    btRaycastVehicle* vehicle;

    // Probably you'll need methods like:
    void ApplyEngineForce(float force);
    void ApplySteer(float value);
    void Brake(float force);
    std::string debugStateSTR();
    btTransform GetTransform() const;  // Useful for rendering

    void ResetTransform();

    float getSpeed() const;

    float getX() const;
    float getY() const;
    float getZ() const;
};

#endif