#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler()
{
}

void Scheduler::addBot(Bot *bot)
{
    m_bots.push(bot);
}

bool Scheduler::cycle()
{
    m_first_start = false;
    std::queue<Bot*> stepped_bots;
    while(m_bots.size() > 0){
        auto current_bot = m_bots.front();
        m_bots.pop();
        bool result = false;
        try{
            result = current_bot->cycle();
        }catch(const std::exception& e){
            std::cerr << e.what() << '\n';
            delete current_bot;
            continue;
        }
        if (result){
            stepped_bots.push(current_bot);
        }
    }
    m_bots = stepped_bots;
    return m_bots.size() > 0;
}

bool Scheduler::isFirstStart()
{
    return m_first_start;
}
