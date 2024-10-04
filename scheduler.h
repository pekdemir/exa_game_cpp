#pragma once
#include <queue>
#include "bot.h"
class Bot;

class Scheduler{
    std::queue<Bot*> m_bots;
    bool m_first_start = true;

public:
    Scheduler();
    void addBot(Bot* bot);
    bool cycle();
    bool isFirstStart();
};