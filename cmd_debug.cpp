#include <iostream>
#include "cmd_debug.h"
#include "globals.h"
#include "room.h"
#include "bot.h"
#include "file.h"

extern Scheduler g_scheduler;
extern Floor g_floor;

CmdDebug::CmdDebug()
{
}

void CmdDebug::run()
{
    std::string cmd;

    std::cout << "> ";
    std::getline(std::cin, cmd);

    // std::cout << cmd << "\n";
    while(cmd != "exit"){
        auto cmd_parts = split(cmd, " ");
        // for (auto part : cmd_parts){
        //     std::cout << part << "\n";
        // }
        // return;
        if (cmd_parts[0] == "help"){
                std::cout << "Commands:\n";
                std::cout << "help - print this help message\n";
                std::cout << "code <bot_id> - enter coding mode for bot\n";
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
        } else if (cmd_parts[0] == "code"){
            if(!g_scheduler.isFirstStart()){
                std::cout << "Code can only be entered before the scheduler is run\n"; 
            }else{
                auto which_bot = g_floor.getEntity(std::stoi(cmd_parts[1]));
                if(which_bot){
                    std::cout << "Enter code for bot " << cmd_parts[1] << ", enter <return> to finish\n";
                    std::string code;
                    std::string code_line;
                    std::cout << "- ";
                    std::getline(std::cin, code_line);
                    while(code_line != ""){
                        code += code_line + "\n";
                        std::cout << "- ";
                        std::getline(std::cin, code_line);
                    }
                    Bot* bot = dynamic_cast<Bot*>(which_bot);
                    if(bot){
                        bot->parseCode(code);
                    }
                }else{
                    std::cout << "Bot not found\n";
                }
            }

        } else if (cmd_parts[0] == "add"){
            if(cmd_parts[1] == "room"){
                g_floor.addRoom(new Room(std::stoi(cmd_parts[2]), std::stoi(cmd_parts[3]), std::stoi(cmd_parts[4])));
            } else if (cmd_parts[1] == "bot"){
                auto which_room = g_floor.getRoom(std::stoi(cmd_parts[3]));
                if(which_room){
                    auto bot = new Bot(std::stoi(cmd_parts[2]));
                    which_room->putEntity(bot);
                    g_scheduler.addBot(bot);
                }else{
                    std::cout << "Room not found\n";
                }
            } else if (cmd_parts[1] == "file"){
                auto which_room = g_floor.getRoom(std::stoi(cmd_parts[3]));
                if(which_room){
                    auto file = new File(std::stoi(cmd_parts[2]));
                    which_room->putEntity(file);
                }else{
                    std::cout << "Room not found\n";
                }
            } else if (cmd_parts[1] == "variable"){
                
            } else if (cmd_parts[1] == "link"){
                auto room1 = g_floor.getRoom(std::stoi(cmd_parts[2]));
                auto room2 = g_floor.getRoom(std::stoi(cmd_parts[3]));
                if(room1 && room2){
                    room1->addLink(std::stoi(cmd_parts[4]), room2);
                } else {
                    std::cout << "Invalid room id\n";
                }
            } else {
                std::cout << "Invalid add parameter\n";
            }
        } else if (cmd_parts[0] == "run"){
            std::cout << "Run\n";
        } else if (cmd_parts[0] == "step"){
            g_scheduler.cycle();
        } else if (cmd_parts[0] == "room"){
            auto which_room = g_floor.getRoom(std::stoi(cmd_parts[1]));
            if(which_room){
                std::cout << which_room->toString();
            }else{
                std::cout << "Room not found\n";
            }
        } else if (cmd_parts[0] == "floor"){
            g_floor.printFloor();
        } else if (cmd_parts[0] == "bot"){
            auto which_bot = g_floor.getEntity(std::stoi(cmd_parts[1]));
            if(which_bot){
                std::cout << which_bot->toString();
                Bot* bot = dynamic_cast<Bot*>(which_bot);
                if(bot) bot->printState();
            }else{
                std::cout << "Bot not found\n";
            }
        } else if (cmd_parts[0] == "file"){
            auto which_file = g_floor.getEntity(std::stoi(cmd_parts[1]));
            if(which_file){
                std::cout << which_file->toString();
            }else{
                std::cout << "File not found\n";
            }
        } else if (cmd_parts[0] == "variable"){
            std::cout << "Variable\n";
        } else {
            std::cout << "Unknown command\n";
        }
        std::cout << "> ";
        std::getline(std::cin, cmd);
    }
}
