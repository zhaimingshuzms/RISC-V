//
// Created by Administrator on 2021/7/6.
//

#ifndef RISC_V_SLBUFFER_HPP
#define RISC_V_SLBUFFER_HPP
#include "Register.hpp"
#include "basic_type.hpp"
#include "rob.h"
#include "../regmem.hpp"
#include "../lib/myqueue.hpp"
#include "../command.hpp"
command_base issue_to_slb(0);
const UINT SLSIZE=32;
class slbufferunit{
public:
    Register<command_base> c;
    Register<UINT> robid;
    Register<UINT> Q1,Q2;
    Register<UINT> V1,V2;
    slbufferunit():c(0){
    }
    slbufferunit(const command_base &c,const UINT &robid,const UINT &Q1,const UINT &Q2,const UINT &V1,const UINT &V2):c(c),robid(robid),Q1(Q1),Q2(Q2),V1(V1),V2(V2){
    }
    bool ready(){
        return (Q1==ROBSIZE||ROB.ready[Q1])
        &&(Q2==ROBSIZE||ROB.ready[Q2]);
    }
    void tick(){
        c.tick();
        robid.tick();
        Q1.tick();
        Q2.tick();
        V1.tick();
        V2.tick();
    }
};
extern command_base issue_to_slb;
class slbuffer{
public:
    myqueue<slbufferunit,SLSIZE> q;
    bool stall;
    void push(){
        if (issue_to_slb.t==command_base::S)
            q.push(slbufferunit(issue_to_slb,reg.Q[issue_to_slb.rd],reg.Q[issue_to_rob.rs1],reg.Q[issue_to_rob.rs2],reg.V[issue_to_rob.rs1],reg.V[issue_to_rob.rs2]));
        else
            q.push(slbufferunit(issue_to_slb,reg.Q[issue_to_slb.rd],reg.Q[issue_to_rob.rs1],ROBSIZE,reg.V[issue_to_rob.rs1],0));
    }
    void run(){
        if (!q.front().ready()){
            stall=true;
            return;
        }
        RSmessage tmp(q.front().c,q.front().V1,q.front().V2,q.front().robid,0);//wrong
        if (q.front().c.prev.opcode==0b0000011){
            Icommand(q.front().c).run(tmp);
        }
        else{
            Scommand(q.front().c).run(tmp);
        }
        q.pop();
    }
    void tick(){
        if (stall) return;
        for (UINT i=q.head; i!=q.tail; i=q.next(i)) q[i].tick();
    }
}slb;
#endif //RISC_V_SLBUFFER_HPP
