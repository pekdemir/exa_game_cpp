#include <stdexcept>
#include "bot.h"
#include <format>
#include <iostream>
#include <fstream>

bool Bot::step()
{
    Instruction instruction = m_instructions[m_registers["PC"]->read()];

    if(instruction.m_opcode == "JUMP"){
        argCheck(instruction, 1);
        if(m_labels.find(instruction.m_args[0]) != m_labels.end()){
            m_registers["PC"]->write(m_labels[instruction.m_args[0]]);
        } else {
            throw std::invalid_argument(std::format("Label {} not found", instruction.m_args[0]));
        }
    }else if(instruction.m_opcode == "LINK"){
        argCheck(instruction, 1);
    }else if(instruction.m_opcode == "COPY"){
        argCheck(instruction, 2);
    }else if(instruction.m_opcode == "ADDI"){
        argCheck(instruction, 3);
    }else if(instruction.m_opcode == "SUBI"){
        argCheck(instruction, 3);
    }else if(instruction.m_opcode == "MULI"){
        argCheck(instruction, 3);
    }else if(instruction.m_opcode == "DIVI"){
        argCheck(instruction, 3);
    }else if(instruction.m_opcode == "GRAB"){
        argCheck(instruction, 1);
    }else if(instruction.m_opcode == "DROP"){
    }else if(instruction.m_opcode == "SEEK"){
        argCheck(instruction, 1);
    }else if(instruction.m_opcode == "HALT"){
        return false;
    }else if(instruction.m_opcode == "KILL"){
        // TODO
    }else if(instruction.m_opcode == "REPL"){
        argCheck(instruction, 1);
    }else if(instruction.m_opcode == "TEST"){
        argCheck(instruction, 3);
    }else if(instruction.m_opcode == "FJMP"){
        argCheck(instruction, 1);
    }else if(instruction.m_opcode == "TJMP"){
        argCheck(instruction, 1);
    }else if(instruction.m_opcode == "MAKE"){
        // TODO
    }else if(instruction.m_opcode == "MARK"){
        // Do nothing
    }else{
        throw std::invalid_argument(std::format("Unknown instruction {}", instruction.m_opcode));
    }


    m_registers["PC"]->write(m_registers["PC"]->read() + 1);
    return false;
}

void Bot::run()
{
    while(step()){
    }
}

std::string Bot::getRegister(std::string arg)
{
    if(arg != "X" || arg != "T" || arg != "F" || arg != "M"){
       throw std::invalid_argument("Invalid register name");
    }
    return m_registers[arg]->name();
}

int Bot::getValue(std::string arg)
{
    if(arg == "X" || arg == "T" || arg == "F" || arg == "M"){
       return m_registers[arg]->read();
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

bool Bot::cycle()
{
    return step();
}

void Bot::exec()
{
    try{
        while (step()){
            printState();
        }
    }
    catch(const std::exception& e){
        std::cerr << e.what() << '\n';
        // destroy the bot
    }
    
    
}

void Bot::printState()
{
    std::cout << "Registers:\n";
    for (auto reg : m_registers){
        if (reg.second){
            std::cout << reg.second->name() << ": " << reg.second->read() << "\n";
        }
    }
    std::cout << "Labels:\n";
    for (auto label : m_labels){
        std::cout << label.first << ":" << label.second << "\n";
    }
    std::cout << "Instructions:\n";
    for(int i = 0; i < m_instructions.size(); i++){
        if(i == m_registers["PC"]->read()){
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

void Bot::parseCodeFromFile(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open()){
        throw std::invalid_argument(std::format("File {} not found", filename));
    }
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    parseCode(code);
}

void Bot::parseCode(std::string code)
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
