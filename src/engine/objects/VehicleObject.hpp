#ifndef VEHICLEPHYSICS_CLASS_H
#define VEHICLEPHYSICS_CLASS_H

#include <btBulletDynamicsCommon.h>  // Assuming you have this included for Bullet physics types
#include "SimulationObject.hpp"

#include "engine/physics/PhysicsWorldSingleton.h"

class VehicleObject final : public SimulationObject  {
private:
    btRigidBody* vehicleRigidBody;
    btVehicleRaycaster* vehicleRayCaster;

    //Bullet manages body interpolation through MotionStates
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
    VehicleObject();
    btRaycastVehicle* vehicle;

    // Probably you'll need methods like:
    void ApplyEngineForce(float force);
    void ApplySteer(float value);
    void Brake(float force);
    std::string debugStateSTR();
    void Update();  // For updating any vehicle logic, physics simulation steps, etc.
    btTransform GetTransform() const;  // Useful for rendering
};

#endif