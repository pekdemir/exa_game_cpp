#pragma once
#include <vector>
#include "bot.h"
class Bot;

class Scheduler{
    std::vector<Bot*> m_bots;
    bool m_first_start = true;

public:
    void addBot(Bot* bot);
    bool cycle();
};