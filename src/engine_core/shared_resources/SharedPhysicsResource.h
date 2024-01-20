#ifndef SHAREDPHYSICSRESOURCE_CLASS_H
#define SHAREDPHYSICSRESOURCE_CLASS_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <mutex>

struct vehiclePhysicsInfo {
    int test;
};


class SharedPhysicsResource {

public:
    void UpdateVehiclePhyInfo(const vehiclePhysicsInfo& data);
    vehiclePhysicsInfo GetVehiclePhyInfo();

private:
    vehiclePhysicsInfo playerVehicle;
    std::mutex mutex_;
};


#endif