#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <any>

enum class EventType
{
    PLAYER_VEHICLE_MOVE,
    PLAYER_VEHICLE_GET_SPEED,

    CAMERA_PLAYER_VEHICLE_FOLLOW,
    CAMERA_TOGGLE_FREE_CAM,

    UPDATE_TERRAIN_CHUNKS_XZ
};

class Event 
{
public:
    EventType type;
    std::any data;

    Event(EventType type, std::any data) : type(type), data(data) {}
};

#endif // EVENT_HPP