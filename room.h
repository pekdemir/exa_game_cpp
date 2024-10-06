#pragma once

#include <vector>
#include <unordered_map>
#include "room_entity.h"

class RoomEntity;

class Room {
    std::string m_id;
    int m_row, m_col;
    std::vector<RoomEntity*> m_slots;
    std::unordered_map<std::string, Room*> m_links;


public:
    Room(const std::string& id, int row, int col);

    std::string getId();
    
    void addLink(const std::string& id, Room* entity);

    Room* getLink(const std::string& link_id);

    bool putEntity(RoomEntity* entity);

    bool removeEntity(RoomEntity* entity);

    RoomEntity* getEntity(const std::string& id);

    std::string toString();
};