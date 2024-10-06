#include <iostream>
#include "floor.h"

Floor::Floor()
{
}

void Floor::addRoom(Room *room)
{
    m_rooms[room->getId()] = room;
}

Room *Floor::getRoom(const std::string& room_id)
{
    auto it = m_rooms.find(room_id);
    if (it != m_rooms.end()){
        return it->second;
    }
    return nullptr;
}

RoomEntity *Floor::getEntity(const std::string& entity_id)
{
    for (auto room : m_rooms){
        auto entity = room.second->getEntity(entity_id);
        if (entity) return entity;
    }
    return nullptr;
}

void Floor::printFloor()
{
     std::cout << "Floor with " << m_rooms.size() << " rooms:\n";
    for (auto room : m_rooms){
        std::cout << room.second->toString() << "\n";
    }
}
