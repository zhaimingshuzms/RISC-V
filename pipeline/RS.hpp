//
// Created by Administrator on 2021/7/5.
//

#ifndef RISC_V_RS_HPP
#define RISC_V_RS_HPP
#include "basic_type.hpp"
command_base issue_to_rs(0);
UINT issue_to_rs_pc;
class RS{
public:
    Register<bool> busy;
    Register<command_base> c;
    Register<UINT> robid;
    Register<UINT> pc;
    Register<UINT> Q1,Q2;
    Register<UINT> V1,V2;
    //有imm就没有rs2
    RS():busy(false),c(0){
    }
    void clear(){
        busy=false;
    }
    void push(command_base &_c,UINT &_pc){
        busy=true;
        c=_c;
        pc=_pc;
        robid=reg.Q[_c.rd];
        Q1=reg.Q[_c.rs1];
        Q2=reg.Q[_c.rs2];
        if (Q1==ROBSIZE||ROB.ready[Q1]) V1=reg.V[_c.rs1]; else V1=UINT_MAX;
        if (Q2==ROBSIZE||ROB.ready[Q2]) V2=reg.V[_c.rs2]; else V2=UINT_MAX;
    }
    bool available(){
        return busy&&V1!=UINT_MAX&&V2!=UINT_MAX;
    }
    RSmessage out(){
        busy=false;
        return RSmessage(c,V1,V2,robid,pc);
    }
    void tick(){
        busy.tick();
        c.tick();
        robid.tick();
        pc.tick();
        Q1.tick();
        Q2.tick();
    }
}rs[8];
#endif //RISC_V_RS_HPP
