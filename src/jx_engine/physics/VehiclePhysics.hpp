#ifndef VEHICLEPHYSICS_CLASS_HPP
#define VEHICLEPHYSICS_CLASS_HPP

#include <bullet/btBulletDynamicsCommon.h>  // Assuming you have this included for Bullet physics types
#include "PhysicsWorldSingleton.h"

class VehiclePhysics {
private:
    btRigidBody* vehicleRigidBody;
    btVehicleRaycaster* vehicleRayCaster;

    btDefaultMotionState *vehicleMotionState;

    btRaycastVehicle::btVehicleTuning tuning;

    btScalar VEHICLE_SCALE;

    float engineForce;
    float vehicleSteering;
    float steeringIncrement;
    float steeringClamp;
    float brakeForce;

    //Will make this slowly incr/decr for steer inputs
    float currentSteer;


public:
    VehiclePhysics();
    btRaycastVehicle* vehicle;

    // Probably you'll need methods like:
    void ApplyEngineForce(float force);
    void ApplySteer(float value);
    void Brake(float force);
    std::string debugStateSTR();
    btTransform GetTransform() const;  // Useful for rendering

    void ResetTransform();

    float getX() const;
    float getY() const;
    float getZ() const;
};

#endif