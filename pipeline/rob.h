//
// Created by Administrator on 2021/7/8.
//

#ifndef RISC_V_ROB_H
#define RISC_V_ROB_H
#include "../lib/sign.h"
#include "basic_type.hpp"
#include "Register.hpp"
UINT code_from_rob_to_commit;
command_base issue_to_rob(0);
UINT issue_to_rob_pc;
UINT EX_to_rob_v;
UINT EX_to_rob_pc;
RSmessage EX_to_rob;
const UINT ROBSIZE=32;//can be modified
class rob{
public:
    UINT head,tail;//左闭右开
    Register<command_base> in[ROBSIZE];
    Register<bool> ready[ROBSIZE];
    Register<UINT> dest[ROBSIZE],value[ROBSIZE],pco[ROBSIZE],pc[ROBSIZE];
    //要在寄存器的dest保存value
    UINT next(UINT x){
        return (x+1)%ROBSIZE;
    }
    rob():head(0),tail(0){
    }
    void clear(){
        head=tail=0;
        memset(ready,0,sizeof(ready));
    }
    bool full(){
        return next(tail)==head;
    }
    void tick(){
        for (auto i:in) i.tick();
        for (auto i:ready) i.tick();
        for (auto i:dest) i.tick();
        for (auto i:value) i.tick();
        for (auto i:pc) i.tick();
    }
}ROB;
#endif //RISC_V_ROB_H
