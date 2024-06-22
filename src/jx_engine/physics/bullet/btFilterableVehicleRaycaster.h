#ifndef BT_FILTERABLE_VEHICLE_RAYCASTER_H
#define BT_FILTERABLE_VEHICLE_RAYCASTER_H

#include <bullet/btBulletDynamicsCommon.h>

class btFilterableVehicleRaycaster : public btDefaultVehicleRaycaster
{
public:

    btDynamicsWorld* m_dynamicsWorld;

    btFilterableVehicleRaycaster(btDynamicsWorld* world);

    short int m_collisionFilterMask;
    short int m_collisionFilterGroup;

    void setCollisionFilterMask(short int collisionFilterMask);
    void setCollisionFilterGroup(short int collisionFilterGroup);

    virtual void* castRay(const btVector3& from, const btVector3& to, btVehicleRaycasterResult& result) override;
};

#endif // BT_FILTERABLE_VEHICLE_RAYCASTER_H
