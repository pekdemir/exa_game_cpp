#pragma once
#include <vector>
#include "room_entity.h"
#include "register_intf.h"


class File: public RoomEntity, public RegisterIntf{
    std::vector<int> m_data;

    int m_index;
    bool m_is_grabbed = false;

public:
    File(int file_id);

    void reset();
    bool isEOF();
    bool isGrabbed();
    void grab();
    void drop();
    void write(int value) override;
    int read() override;
    void seek(int offset);
    std::string toString() override;

};
  