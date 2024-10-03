#pragma once
#include <vector>
#include <string>
#include "floor.h"
#include "scheduler.h"

extern std::vector<std::string> split(const std::string& str, const std::string& delim){
    std::vector<std::string> result;
    size_t start = 0;

    for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, start))
    {
        result.emplace_back(str.begin() + start, str.begin() + found);
        start = found + delim.size();
    }
    if (start != str.size())
        result.emplace_back(str.begin() + start, str.end());
    return result;
}

// Globals
static Scheduler g_scheduler;
static Floor g_floor;
