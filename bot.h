#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <optional>
#include <format>
#include "globals.h"
#include "room_entity.h"
#include "register_intf.h"

class Bot: public RoomEntity{
    struct Instruction{
        std::string m_opcode;
        std::vector<std::string> m_args;

        Instruction(std::string instruction_str){
            auto parts = split(instruction_str, " ");
            m_opcode = parts[0];
            for (size_t i = 1; i < parts.size(); i++){
                m_args.push_back(parts[i]);
            }
        }
    };

    class Register: public RegisterIntf{
    public:
        Register(std::string name): RegisterIntf(name){
        }

        void write(int value) override{
            m_value = value;
        }

        int read() override{
            return m_value;
        }
    };

    std::vector<Instruction> m_instructions;
    std::unordered_map<std::string, std::optional<RegisterIntf>> m_registers{
        {"X", Register("X")},
        {"T", Register("T")},
        {"F", std::nullopt},
        {"M", std::nullopt},
        {"PC", Register("PC")}      
    };
    std::unordered_map<std::string, int> m_labels;

    bool step();
    void run();
    std::string getRegister(std::string arg);
    int getValue(std::string arg);
    void argCheck(Instruction instruction, int arg_count);

public:
    Bot(int bot_id);
    bool cycle();
    void exec();
    void printState();
    void parseCodeFromFile(std::string filename);
    void parseCode(std::string code);

    std::string toString() override{
        return std::format("Bot ID:{}", getId());
    }

};