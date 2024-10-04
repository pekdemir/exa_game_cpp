#pragma once

#include "room.h"

class Room;

class RoomEntity {
protected:
    int m_id;
    Room* m_room;

public:
    RoomEntity(int id);

    void setRoom(Room* room);

    Room* getRoom();

    int getId();

    bool move(int link_id);

    virtual std::string toString() = 0;
};