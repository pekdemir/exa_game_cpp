#pragma once

#include <vector>
#include <unordered_map>
#include "room_entity.h"

class RoomEntity;

class Room {
    int m_id, m_row, m_col;
    std::vector<RoomEntity*> m_slots;
    std::unordered_map<int, Room*> m_links;


public:
    Room(int id, int row, int col);

    int getId();
    
    void addLink(int id, Room* entity);

    Room* getLink(int link_id);

    bool putEntity(RoomEntity* entity);

    bool removeEntity(RoomEntity* entity);

    RoomEntity* getEntity(int id);

    std::string toString();
};