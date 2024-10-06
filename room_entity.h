#pragma once

#include "room.h"

class Room;

class RoomEntity {
protected:
    std::string m_id;
    Room* m_room;

public:
    RoomEntity(const std::string& id);

    void setRoom(Room* room);

    Room* getRoom();

    const std::string& getId();

    bool move(const std::string& link_id);

    virtual std::string toString() = 0;
};