#include <iostream>
#include <fstream>
#include "cmd_debug.h"
#include "globals.h"
#include "room.h"
#include "bot.h"
#include "file.h"
#include "room_variable.h"
#include "picojson.h"

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
    std::cin.clear();

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
                std::cout << "load <json_file> - load floor config from JSON\n";
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
                auto which_bot = g_floor.getEntity(cmd_parts[1]);
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
        } else if (cmd_parts[0] == "load"){
            auto file_path = cmd_parts[1];
            std::ifstream input(file_path);
            if(input.is_open()){
                picojson::value v;
                std::string err;
                std::istream_iterator<char> begin(input);
                std::istream_iterator<char> end;
                auto output = picojson::parse(v, begin, end, &err);
                if (! err.empty()) {
                    std::cerr << err << std::endl;
                }
                std::cout << v.to_str() << "\n";
                auto& obj = v.get<picojson::object>();
                auto& rooms = obj["Rooms"].get<picojson::array>();
                for (auto& room : rooms){
                    auto& room_obj = room.get<picojson::object>();
                    auto room_id = room_obj["id"].get<std::string>();
                    auto row = room_obj["row"].get<std::string>();
                    auto col = room_obj["col"].get<std::string>();
                    g_floor.addRoom(new Room(room_id, std::stoi(row), std::stoi(col)));
                }
                auto& links = obj["Links"].get<picojson::array>();
                for (auto& link : links){
                    auto& link_obj = link.get<picojson::object>();
                    auto room1 = link_obj["from"].get<std::string>();
                    auto room2 = link_obj["to"].get<std::string>();
                    auto link_id = link_obj["id"].get<std::string>();
                    auto room1_ptr = g_floor.getRoom(room1);
                    auto room2_ptr = g_floor.getRoom(room2);
                    if(room1_ptr && room2_ptr){
                        room1_ptr->addLink(link_id, room2_ptr);
                    }
                }
                auto& bots = obj["Bots"].get<picojson::array>();
                for (auto& bot : bots){
                    auto& bot_obj = bot.get<picojson::object>();
                    auto bot_id = bot_obj["id"].get<std::string>();
                    auto room_id = bot_obj["room"].get<std::string>();
                    auto room_ptr = g_floor.getRoom(room_id);
                    if(room_ptr){
                        auto bot_ptr = new Bot(bot_id);
                        room_ptr->putEntity(bot_ptr);
                        g_scheduler.addBot(bot_ptr);
                    }
                }
                auto& files = obj["Files"].get<picojson::array>();
                for (auto& file : files){
                    auto& file_obj = file.get<picojson::object>();
                    auto file_id = file_obj["id"].get<std::string>();
                    auto room_id = file_obj["room"].get<std::string>();
                    // TODO:extract data
                    auto room_ptr = g_floor.getRoom(room_id);
                    if(room_ptr){
                        auto file_ptr = new File(file_id);
                        room_ptr->putEntity(file_ptr);
                    }
                }
                auto& variables = obj["Variables"].get<picojson::array>();
                for (auto& variable : variables){
                    auto& variable_obj = variable.get<picojson::object>();
                    auto variable_id = variable_obj["id"].get<std::string>();
                    auto room_id = variable_obj["room"].get<std::string>();
                    auto value = variable_obj["value"].get<std::string>();
                    auto read_only = variable_obj["read_only"].get<std::string>();
                    auto room_ptr = g_floor.getRoom(room_id);
                    if(room_ptr){
                        auto variable_ptr = new RoomVariable(variable_id, true ? read_only == "true" : false);
                        variable_ptr->write(std::stoi(value));
                        room_ptr->putEntity(variable_ptr);
                    }
                }

            }else{
                std::cout << "File not found\n";
            }
        } else if (cmd_parts[0] == "add"){
            if(cmd_parts[1] == "room"){
                g_floor.addRoom(new Room(cmd_parts[2], std::stoi(cmd_parts[3]), std::stoi(cmd_parts[4])));
            } else if (cmd_parts[1] == "bot"){
                auto which_room = g_floor.getRoom(cmd_parts[3]);
                if(which_room){
                    auto bot = new Bot(cmd_parts[2]);
                    which_room->putEntity(bot);
                    g_scheduler.addBot(bot);
                }else{
                    std::cout << "Room not found\n";
                }
            } else if (cmd_parts[1] == "file"){
                auto which_room = g_floor.getRoom(cmd_parts[3]);
                if(which_room){
                    auto file = new File(cmd_parts[2]);
                    which_room->putEntity(file);
                }else{
                    std::cout << "Room not found\n";
                }
            } else if (cmd_parts[1] == "variable"){
                auto which_room = g_floor.getRoom(cmd_parts[3]);
                if(which_room){
                    auto variable = new RoomVariable(cmd_parts[2], true ? cmd_parts[4] == "true" : false);
                    which_room->putEntity(variable);
                }else{
                    std::cout << "Room not found\n";
                }
            } else if (cmd_parts[1] == "link"){
                auto room1 = g_floor.getRoom(cmd_parts[2]);
                auto room2 = g_floor.getRoom(cmd_parts[3]);
                if(room1 && room2){
                    room1->addLink(cmd_parts[4], room2);
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
            auto which_room = g_floor.getRoom(cmd_parts[1]);
            if(which_room){
                std::cout << which_room->toString();
            }else{
                std::cout << "Room not found\n";
            }
        } else if (cmd_parts[0] == "floor"){
            g_floor.printFloor();
        } else if (cmd_parts[0] == "bot"){
            auto which_bot = g_floor.getEntity(cmd_parts[1]);
            if(which_bot){
                std::cout << which_bot->toString();
                Bot* bot = dynamic_cast<Bot*>(which_bot);
                if(bot) bot->printState();
            }else{
                std::cout << "Bot not found\n";
            }
        } else if (cmd_parts[0] == "file"){
            auto which_file = g_floor.getEntity(cmd_parts[1]);
            if(which_file){
                std::cout << which_file->toString() << "\n";
            }else{
                std::cout << "File not found\n";
            }
        } else if (cmd_parts[0] == "variable"){
            auto which_variable = g_floor.getEntity(cmd_parts[1]);
            if(which_variable){
                std::cout << which_variable->toString() << "\n";
            }else{
                std::cout << "Variable not found\n";
            }
        } else {
            std::cout << "Unknown command\n";
        }
        std::cout << "> ";
        std::getline(std::cin, cmd);
        std::cin.clear();
    }
}
