#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <format>
#include "globals.h"
#include "room_entity.h"
#include "register_intf.h"

extern std::vector<std::string> split(const std::string& str, const std::string& delim);

class Bot: public RoomEntity{

    enum class ERegister{
        X = 1,
        T,
        F,
        M,
        PC,
        undefined
    };

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

        static ERegister convertName(const std::string& name){
            if (name == "X"){
                return ERegister::X;
            }else if (name == "T"){
                return ERegister::T;
            }else if (name == "F"){
                return ERegister::F;
            }else if (name == "M"){
                return ERegister::M;
            }else if (name == "PC"){
                return ERegister::PC;
            }
            return ERegister::undefined;
        }

        void write(int value) override{
            m_value = value;
        }

        int read() override{
            return m_value;
        }
    };

    std::vector<Instruction> m_instructions;
    std::unordered_map<ERegister, RegisterIntf*> m_registers{
        {ERegister::X, dynamic_cast<RegisterIntf*>(new Register("X"))},
        {ERegister::T, dynamic_cast<RegisterIntf*>(new Register("T"))},
        {ERegister::F, nullptr},
        {ERegister::M, nullptr},
        {ERegister::PC, dynamic_cast<RegisterIntf*>(new Register("PC"))}
    };
    std::unordered_map<std::string, int> m_labels;

    bool step();
    void run();
    ERegister getRegister(const std::string& arg);
    int getValue(const std::string& arg);
    void argCheck(Instruction instruction, int arg_count);

public:
    Bot(int bot_id);
    ~Bot();
    bool cycle();
    void exec();
    void printState();
    void parseCodeFromFile(const std::string& filename);
    void parseCode(const std::string& code);

    std::string toString() override;

};