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

    void SwapBuffers();

private:
    vehiclePhysicsInfo playerVehicleBuffer[2];
    int rI = 0; //Read 
    int wI = 1; //Write

    std::mutex mutex_;
    btDiscreteDynamicsWorld* physicsWorld_;

};


#endif