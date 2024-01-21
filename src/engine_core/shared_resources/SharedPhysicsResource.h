#ifndef SHAREDPHYSICSRESOURCE_CLASS_H
#define SHAREDPHYSICSRESOURCE_CLASS_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>


#include <mutex>

struct vehiclePhysicsInfo {
    btTransform transform;
};




class SharedPhysicsResource {

public:
    void UpdateVehiclePhyInfo(const vehiclePhysicsInfo& data);
    vehiclePhysicsInfo GetVehiclePhyInfo();

    void UpdatePhysicsWorld(btDiscreteDynamicsWorld* physicsWorld);
    btDiscreteDynamicsWorld* getPhysicsWorld();

private:
    vehiclePhysicsInfo playerVehicle;
    std::mutex mutex_;
    btDiscreteDynamicsWorld* physicsWorld_;

};


#endif