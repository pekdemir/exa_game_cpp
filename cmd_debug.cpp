#include <iostream>
#include "cmd_debug.h"
#include "globals.h"

CmdDebug::CmdDebug()
{
}

void CmdDebug::run()
{
    std::string cmd;

    std::cout << "> ";
    std::cin >> cmd;
    while(cmd != "exit"){
        auto cmd_parts = split(cmd, " ");
        if (cmd_parts[0] == "help"){
                std::cout << "Commands:\n";
                std::cout << "help - print this help message\n";
                std::cout << "add - add an entity to the floor\n";
                std::cout << "    room <room_id> <row> <col> - add a room to the floor\n";
                std::cout << "    bot <bot_id> <room_id> - add a bot to the room\n";
                std::cout << "    file <file_id> <room_id> - add a file to the room\n";
                std::cout << "    variable <variable_id> <data> <is_persistent> <room_id>- add a variable to the room\n";
                std::cout << "    link <room_id1> <room_id2> <link_id> - add a link between rooms\n";
                std::cout << "run <count> - run the scheduler for <count> cycles\n";
                std::cout << "step - run one step of the scheduler\n";
                std::cout << "room <room_id> - print room details\n";
                std::cout << "floor - print floor details\n";
                std::cout << "bot <bot_id> - print bot details\n";
                std::cout << "file <file_id> - print file details\n";
                std::cout << "variable <variable_id> - print variable details\n";
                std::cout << "exit - exit the program\n";
        } else if (cmd_parts[0] == "run"){
            std::cout << "Run\n";
        } else if (cmd_parts[0] == "stop"){
            std::cout << "Stop\n";
        } else {
            std::cout << "Unknown command\n";
        }
        std::cout << "> ";
        std::cin >> cmd;
    }
}
