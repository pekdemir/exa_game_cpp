#include "room_entity.h"

RoomEntity::RoomEntity(const std::string& id): m_id(id), m_room(nullptr)
{
}

void RoomEntity::setRoom(Room *room)
{
    m_room = room;
}

Room *RoomEntity::getRoom()
{
    return m_room;
}

const std::string& RoomEntity::getId()
{
    return m_id;
}

bool RoomEntity::move(const std::string& link_id)
{
    // search all links of the current room
    auto new_room = m_room->getLink(link_id);
    if (new_room) {
        // move to the linked room
        if(new_room->putEntity(this)) {
            m_room->removeEntity(this);
            m_room = new_room;
            return true;
        }
    }
    return false;
}
