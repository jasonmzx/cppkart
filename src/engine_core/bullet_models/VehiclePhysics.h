#ifndef VEHICLEPHYSICS_CLASS_H
#define VEHICLEPHYSICS_CLASS_H

#include <btBulletDynamicsCommon.h>  // Assuming you have this included for Bullet physics types

class VehiclePhysics {
private:
    btRigidBody* vehicleRigidBody;
    btVehicleRaycaster* vehicleRayCaster;

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
    void printState();
    void Update();  // For updating any vehicle logic, physics simulation steps, etc.
    btTransform GetTransform() const;  // Useful for rendering
};

#endif