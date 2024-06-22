#include "btFilterableVehicleRaycaster.h"

btFilterableVehicleRaycaster::btFilterableVehicleRaycaster(btDynamicsWorld* world)
    : btDefaultVehicleRaycaster(world), 
      m_collisionFilterMask(btBroadphaseProxy::AllFilter),
      m_collisionFilterGroup(btBroadphaseProxy::DefaultFilter)
{
    m_dynamicsWorld = world;
}

void btFilterableVehicleRaycaster::setCollisionFilterMask(short int collisionFilterMask)
{
    m_collisionFilterMask = collisionFilterMask;
}

void btFilterableVehicleRaycaster::setCollisionFilterGroup(short int collisionFilterGroup)
{
    m_collisionFilterGroup = collisionFilterGroup;
}

void* btFilterableVehicleRaycaster::castRay(const btVector3& from, const btVector3& to, btVehicleRaycasterResult& result)
{
    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

    rayCallback.m_collisionFilterMask = m_collisionFilterMask;
    rayCallback.m_collisionFilterGroup = m_collisionFilterGroup;

    m_dynamicsWorld->rayTest(from, to, rayCallback);

    if (rayCallback.hasHit())
    {
        const btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
        if (body && body->hasContactResponse())
        {
            result.m_hitPointInWorld = rayCallback.m_hitPointWorld;
            result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;
            result.m_hitNormalInWorld.normalize();
            result.m_distFraction = rayCallback.m_closestHitFraction;
            return (void*)body;
        }
    }
    return nullptr;
}
