#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler()
{
}

void Scheduler::addBot(Bot *bot)
{
    m_bots.push_back(bot);
}

bool Scheduler::cycle()
{
    m_first_start = false;
    std::vector<Bot*> stepped_bots;
    while(m_bots.size() > 0){
        auto current_bot = m_bots.front();
        bool result = false;
        try{
            result = current_bot->cycle();
        }catch(const std::exception& e){
            std::cerr << e.what() << '\n';
            // destroy the current bot
            continue;
        }
        if (result){
            stepped_bots.push_back(current_bot);
        }
    }
    m_bots = stepped_bots;
    return m_bots.size() > 0;
}
