#pragma once

#include <unordered_map>
#include "room.h"


class Floor{
    std::unordered_map<int, Room*> m_rooms;

public:
    Floor();

    void addRoom(Room* room);
    
    Room* getRoom(int room_id);

    RoomEntity* getEntity(int entity_id);

    void printFloor();
};
