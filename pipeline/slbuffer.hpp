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
UINT issue_to_slb_robid;
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
        return (Q1==ROBSIZE)&&(Q2==ROBSIZE);
    }
    void tick(){
        //std::cerr<<"SLB TICK"<<std::endl;
        c.tick();
        robid.tick();
        Q1.tick();
        Q2.tick();
        V1.tick();
        V2.tick();
    }
    void flush(){
        //std::cerr<<"SLB flush"<<c.prev.rs1<<" "<<c.prev.rs2<<" "<<reg.Q[c.prev.rs1]<<std::endl;
        //std::cerr<<"SLB flush"<<c.prev.origin_code<<" "<<Q1.prev<<std::endl;
        if (Q1<ROBSIZE&&ROB.ready[Q1]) V1=ROB.value[Q1],Q1=ROBSIZE;// 0 寄存器的问题还需考虑
        if (Q2<ROBSIZE&&ROB.ready[Q2]) V2=ROB.value[Q2],Q2=ROBSIZE;
        if (Q1<ROBSIZE&&reg.Q(c.prev.rs1)==ROBSIZE) V1=reg[c.prev.rs1],Q1=ROBSIZE;
        if (Q2<ROBSIZE&&reg.Q(c.prev.rs2)==ROBSIZE) V2=reg[c.prev.rs2],Q2=ROBSIZE;
    }
};
bool lacommit;
class slbuffer{
public:
    myqueue<slbufferunit,SLSIZE> q;
    void push(){
        //std::cerr<<"------------------------SLB push"<<issue_to_slb.origin_code<<" "<<issue_to_slb.rs1<<" "<<reg.Q(issue_to_slb.rs1)<<std::endl;
        if (issue_to_slb.t==command_base::S)
            q.push(slbufferunit(issue_to_slb,issue_to_slb_robid,reg.Q(issue_to_rob.rs1),reg.Q(issue_to_rob.rs2),reg[issue_to_rob.rs1],reg[issue_to_rob.rs2]));
        else
            q.push(slbufferunit(issue_to_slb,issue_to_slb_robid,reg.Q(issue_to_rob.rs1),ROBSIZE,reg[issue_to_rob.rs1],0));
    }
    void run(){
        if (!q.front().ready()) return;
        if (q.front().c.prev.opcode==0b0000011&&!lacommit) return;
        //std::cerr<<"slbrun : "<<q.front().c.prev.origin_code<<" "<<q.front().V1<<" "<<q.front().V2<<std::endl;
        RSmessage tmp(q.front().c,q.front().V1,q.front().V2,q.front().robid,0);//wrong
        if (q.front().c.prev.opcode==0b0000011){
            Icommand(q.front().c.prev.origin_code).run(tmp);
        }
        else{
            Scommand(q.front().c.prev.origin_code).run(tmp);
            lacommit=false;//上一个store commit 完
        }
        //std::cerr<<"slbrun result:"<<SLB_to_rob_d<<" "<<SLB_to_rob_v<<std::endl;
        q.pop();
    }
    void tick(){
        //std::cerr<<"outer slb TICK-----------"<<q.head<<" "<<q.tail<<std::endl;
        for (UINT i=q.head; i!=q.tail; i=q.next(i)) q[i].tick();
    }
    void flush(){
        for (UINT i=q.head; i!=q.tail; i=q.next(i)) q[i].flush();
    }
}slb;
#endif //RISC_V_SLBUFFER_HPP
