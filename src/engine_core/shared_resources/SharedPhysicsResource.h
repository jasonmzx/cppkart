#ifndef SHAREDPHYSICSRESOURCE_CLASS_H
#define SHAREDPHYSICSRESOURCE_CLASS_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <mutex>
#include <vector>

struct vehiclePhysicsInfo {
    btTransform transform;
    std::vector<btWheelInfo> wheelVec;
};

class SharedPhysicsResource {

public:
    void UpdateVehiclePhyInfo(const vehiclePhysicsInfo& data);
    vehiclePhysicsInfo GetVehiclePhyInfo();

    void SwapBuffers();

    void UpdatePhysicsWorld(btDiscreteDynamicsWorld* physicsWorld);
    btDiscreteDynamicsWorld* getPhysicsWorld();

private:
    vehiclePhysicsInfo playerVehicleBuffer[2];
    int rI = 0; //Read 
    int wI = 1; //Write

    std::mutex mutex_;
    btDiscreteDynamicsWorld* physicsWorld_;
};


#endif