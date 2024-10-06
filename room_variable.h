#include "room_entity.h"

class RoomVariable: public RoomEntity{

    int m_value;
    bool m_is_read_only;

public:
    RoomVariable(const std::string& room_var_id);
    ~RoomVariable();

    int read();
    void write(int value);
    std::string toString() override;
};