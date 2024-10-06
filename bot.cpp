#include <stdexcept>
#include <format>
#include <iostream>
#include <fstream>
#include "bot.h"
#include "file.h"

bool Bot::step()
{
    Instruction instruction = m_instructions[m_registers[ERegister::PC]->read()];

    if(instruction.m_opcode == "JUMP"){
        argCheck(instruction, 1);
        if(m_labels.find(instruction.m_args[0]) != m_labels.end()){
            m_registers[ERegister::PC]->write(m_labels[instruction.m_args[0]]);
        } else {
            throw std::runtime_error(std::format("Label {} not found", instruction.m_args[0]));
        }
    }else if(instruction.m_opcode == "LINK"){
        argCheck(instruction, 1);
        int link_id = getValue(instruction.m_args[0]);
        if(!move(link_id)){
            throw std::runtime_error(std::format("Link {} not found", link_id));
        }
        if(m_registers[ERegister::F]){
            File* file = dynamic_cast<File*>(m_registers[ERegister::F]);
            if(file && !file->move(link_id)){
                throw std::runtime_error(std::format("Link {} not found", link_id));
            }
        }
    }else if(instruction.m_opcode == "COPY"){
        argCheck(instruction, 2);
        auto dest = getRegister(instruction.m_args[1]);
        auto source = getValue(instruction.m_args[0]);
        m_registers[dest]->write(source);
    }else if(instruction.m_opcode == "ADDI"){
        argCheck(instruction, 3);
        auto dest = getRegister(instruction.m_args[2]);
        auto first = getValue(instruction.m_args[0]);
        auto second = getValue(instruction.m_args[1]);
        m_registers[dest]->write(first + second);
    }else if(instruction.m_opcode == "SUBI"){
        argCheck(instruction, 3);
        auto dest = getRegister(instruction.m_args[2]);
        auto first = getValue(instruction.m_args[0]);
        auto second = getValue(instruction.m_args[1]);
        m_registers[dest]->write(first - second);
    }else if(instruction.m_opcode == "MULI"){
        argCheck(instruction, 3);
        auto dest = getRegister(instruction.m_args[2]);
        auto first = getValue(instruction.m_args[0]);
        auto second = getValue(instruction.m_args[1]);
        m_registers[dest]->write(first * second);
    }else if(instruction.m_opcode == "DIVI"){
        argCheck(instruction, 3);
        auto dest = getRegister(instruction.m_args[2]);
        auto first = getValue(instruction.m_args[0]);
        auto second = getValue(instruction.m_args[1]);
        m_registers[dest]->write(first / second);
    }else if(instruction.m_opcode == "GRAB"){
        argCheck(instruction, 1);
        int file_id = getValue(instruction.m_args[0]);
        auto file_entity = m_room->getEntity(file_id);
        File* file = dynamic_cast<File*>(file_entity);
        if(file){
            if(file->isGrabbed()){
                throw std::runtime_error(std::format("File {} already grabbed", file_id));
            }
            file->reset();
            file->grab();
            m_registers[ERegister::F] = file;
        } else {
            throw std::runtime_error(std::format("File {} not found", file_id));
        }
    }else if(instruction.m_opcode == "DROP"){
        if(m_registers[ERegister::F]){
            File* file = dynamic_cast<File*>(m_registers[ERegister::F]);
            if(file){
                file->drop();
                m_registers[ERegister::F] = nullptr;
            }
        }else{
            throw std::runtime_error("No file grabbed");
        }
    }else if(instruction.m_opcode == "SEEK"){
        argCheck(instruction, 1);
        if(m_registers[ERegister::F]){
            File* file = dynamic_cast<File*>(m_registers[ERegister::F]);
            if(file){
                file->seek(getValue(instruction.m_args[0]));
            }
        }else{
            throw std::runtime_error("No file grabbed");
        }
    }else if(instruction.m_opcode == "HALT"){
        return false;
    }else if(instruction.m_opcode == "KILL"){
        // TODO
    }else if(instruction.m_opcode == "REPL"){
        argCheck(instruction, 1);
        // TODO
    }else if(instruction.m_opcode == "TEST"){
        argCheck(instruction, 3);
        auto first = getRegister(instruction.m_args[0]);
        auto second_value = getValue(instruction.m_args[2]);
        auto op = instruction.m_args[1];
        auto first_value = m_registers[first]->read();
        if(op == "=="){
            m_registers[ERegister::T]->write(first_value == second_value);
        }else if(op == "!="){
            m_registers[ERegister::T]->write(first_value != second_value);
        }else if(op == "<"){
            m_registers[ERegister::T]->write(first_value < second_value);
        }else if(op == ">"){
            m_registers[ERegister::T]->write(first_value > second_value);
        }else if(op == "<="){
            m_registers[ERegister::T]->write(first_value <= second_value);
        }else if(op == ">="){
            m_registers[ERegister::T]->write(first_value >= second_value);
        }else{
            throw std::invalid_argument(std::format("Invalid operator {}", op));
        }
    }else if(instruction.m_opcode == "FJMP"){
        argCheck(instruction, 1);
        if(m_registers[ERegister::T]->read() == 0){
            if(m_labels.find(instruction.m_args[0]) != m_labels.end()){
                m_registers[ERegister::PC]->write(m_labels[instruction.m_args[0]]);
            } else {
                throw std::runtime_error(std::format("Label {} not found", instruction.m_args[0]));
            }
        }
    }else if(instruction.m_opcode == "TJMP"){
        argCheck(instruction, 1);
        if(m_registers[ERegister::T]->read() != 0){
            if(m_labels.find(instruction.m_args[0]) != m_labels.end()){
                m_registers[ERegister::PC]->write(m_labels[instruction.m_args[0]]);
            } else {
                throw std::runtime_error(std::format("Label {} not found", instruction.m_args[0]));
            }
        }
    }else if(instruction.m_opcode == "MAKE"){
        // TODO
    }else if(instruction.m_opcode == "MARK"){
        // Do nothing
    }else{
        throw std::runtime_error(std::format("Unknown instruction {}", instruction.m_opcode));
    }


    m_registers[ERegister::PC]->write(m_registers[ERegister::PC]->read() + 1);
    return true;
}

void Bot::run()
{
    while(step()){
    }
}

Bot::ERegister Bot::getRegister(const std::string& arg)
{
    ERegister reg = Register::convertName(arg);
    if(reg == ERegister::undefined){
       throw std::invalid_argument("Invalid register name");
    }
    return reg;
}

int Bot::getValue(const std::string& arg)
{
    ERegister reg = Register::convertName(arg);
    if(reg != ERegister::undefined){
       return m_registers[reg]->read();
    }
    return std::stoi(arg);
}

void Bot::argCheck(Instruction instruction, int arg_count)
{
    if(instruction.m_args.size() != arg_count){
        throw std::invalid_argument(std::format("Instruction {} requires {} arguments", instruction.m_opcode, arg_count));
    }
}

Bot::Bot(int bot_id): RoomEntity(bot_id)
{
}

Bot::~Bot()
{
    std::cout << "Bot " << m_id << " destroyed\n";
    for(auto reg : m_registers){
        if(reg.second){
            delete reg.second;
            reg.second = nullptr;
        }
    }
}

bool Bot::cycle()
{
    return step();
}

void Bot::printState()
{
    std::cout << "Registers:\n";
    for (auto reg : m_registers){
        if (reg.second){
            std::cout << reg.second->toString() << "\n";
        }
    }
    std::cout << "Labels:\n";
    for (auto label : m_labels){
        std::cout << label.first << ":" << label.second << "\n";
    }
    std::cout << "Instructions:\n";
    for(int i = 0; i < m_instructions.size(); i++){
        if(i == m_registers[ERegister::PC]->read()){
            std::cout << ">";
        } else {
            std::cout << " ";
        }
        std::cout << m_instructions[i].m_opcode;
        for (auto arg : m_instructions[i].m_args){
            std::cout << " " << arg;
        }
        std::cout << "\n";
    }
}

void Bot::parseCodeFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()){
        throw std::runtime_error(std::format("File {} not found", filename));
    }
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    parseCode(code);
}

void Bot::parseCode(const std::string& code)
{
    m_instructions.clear();
    m_labels.clear();
    auto lines = split(code, "\n");
    int index = 0;
    for (auto line : lines){
        m_instructions.emplace_back(line);
        if(m_instructions.back().m_opcode == "MARK"){
            m_labels[m_instructions.back().m_args[0]] = index;
        }
        index++;
    }
    printState();
}

std::string Bot::toString(){
    return std::format("Bot ID:{}\n", getId());
}
