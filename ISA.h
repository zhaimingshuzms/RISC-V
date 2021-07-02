//
// Created by Administrator on 2021/6/29.
//

#ifndef RISC_V_ISA_HPP
#define RISC_V_ISA_HPP
#include <string.h>
#include <iostream>
#include "memory.hpp"
#include "regmem.hpp"
#include "ALU.hpp"
#include "LOGIC.hpp"
typedef unsigned int UINT;
typedef std::string STR;
class input_traits{
    UINT val;
public:
    input_traits(const STR &);
    UINT getval();
};
class ISA{
public:
    memory *mem;
    regmem *reg;
    ALU *alu;
    LOGIC *logic;
    ISA():mem(new memory()),reg(new regmem()),alu(new ALU),logic(new LOGIC){
    }
    void read();
    void run();
};
#endif //RISC_V_ISA_HPP
