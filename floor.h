#pragma once

#include <unordered_map>
#include "room.h"


class Floor{
    std::unordered_map<std::string, Room*> m_rooms;

public:
    Floor();

    void addRoom(Room* room);
    
    Room* getRoom(const std::string& room_id);

    RoomEntity* getEntity(const std::string& entity_id);

    void printFloor();
};
