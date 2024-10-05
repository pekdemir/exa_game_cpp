#pragma once
#include <string>

class RegisterIntf{
protected:
    std::string m_name;
    int m_value;
public:
    RegisterIntf(std::string name): m_name(name), m_value(0){
    }

    std::string name(){
        return m_name;
    }

    virtual void write(int value){
        m_value = value;
    }

    virtual int read(){
        return m_value;
    }

    virtual std::string toString() = 0;
};