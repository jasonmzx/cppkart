#ifndef VEHICLEENTITIY_CLASS_H
#define VEHICLEENTITIY_CLASS_H

//Bullet Physics Engine Imports:

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <bullet/BulletDynamics/Vehicle/btRaycastVehicle.h>


class VehicleEntity {

public:
    void Initialize();
    void Update();
};

#endif