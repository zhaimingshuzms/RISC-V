//
// Created by Administrator on 2021/7/5.
//

#ifndef RISC_V_RS_HPP
#define RISC_V_RS_HPP
#include "basic_type.hpp"
command_base issue_to_rs(0);
UINT issue_to_rs_pc;
UINT issue_to_rs_robid;
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
        memset(this,0,sizeof(*this));
        Q1.prev=Q1.next=Q2.prev=Q2.next=ROBSIZE;
        V1.prev=V1.next=V2.prev=V2.next=UINT_MAX;
    }
    void push(command_base &_c,UINT &_pc){
        //RS 可能在regmem 前
        //std::cerr<<"init push"<<_c.origin_code<<" "<<_c.rs1<<" "<<_c.rs2<<std::endl;
        busy=true;
        c=_c;
        pc=_pc;
        robid=issue_to_rs_robid;
        Q1=reg.Q(_c.rs1);
        Q2=reg.Q(_c.rs2);
        if (_c.opcode==0b0010011){
            if (_c.opcode2==0b000||_c.opcode2==0b010||_c.opcode2==0b011||_c.opcode2==0b100||_c.opcode2==0b110||_c.opcode2==0b111) Q2=ROBSIZE;
        }
        if (_c.opcode==0b1100111){
            Q2=ROBSIZE;
        }
        if (_c.opcode==0b1101111||_c.opcode==0b0110111||_c.opcode==0b0010111){
            Q1=ROBSIZE;
            Q2=ROBSIZE;
        }
        //0 寄存器的问题
        /*if (Q1.next<ROBSIZE&&_c.rs1==0){
            Q1=ROBSIZE;
            V1=0;
        }
        if (Q2.next<ROBSIZE&&_c.rs2==0){
            Q2=ROBSIZE;
            V2=0;
        }*/
        if (Q1.next==ROBSIZE||ROB.ready[Q1.next]) V1=reg[_c.rs1]; else V1=UINT_MAX;
        if (Q2.next==ROBSIZE||ROB.ready[Q2.next]) V2=reg[_c.rs2]; else V2=UINT_MAX;
        //std::cerr<<"init push end"<<Q1.next<<" "<<V1.next<<std::endl;
    }
    bool available(){
        return busy&&Q1==ROBSIZE&&Q2==ROBSIZE;
    }
    RSmessage out(){
        //std::cerr<<"RSOUT"<<c.prev.origin_code<<" "<<V1.prev<<" "<<V1.next<<" "<<Q1.prev<<" "<<Q1.next<<std::endl;
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
        V1.tick();
        V2.tick();
    }
    void flush(){
        //std::cerr<<"RS flush"<<c.prev.origin_code<<" "<<busy<<" "<<Q1<<" "<<Q2<<" "<<V1<<" "<<V2<<" "<<reg.Q[1]<<std::endl;
        if (c.prev.t!=command_base::J&&c.prev.t!=command_base::B&&c.prev.opcode!=0b1100111) {
            if (busy && Q1 < ROBSIZE && ROB.ready[Q1]) V1 = ROB.value[Q1], Q1 = ROBSIZE;
            if (busy && Q2 < ROBSIZE && ROB.ready[Q2]) V2 = ROB.value[Q2], Q2 = ROBSIZE;
        }
        if (busy&&Q1<ROBSIZE&&reg.Q(c.prev.rs1)==ROBSIZE) V1=reg[c.prev.rs1],Q1=ROBSIZE;
        if (busy&&Q2<ROBSIZE&&reg.Q(c.prev.rs2)==ROBSIZE) V2=reg[c.prev.rs2],Q2=ROBSIZE;
    }
}rs[8];
#endif //RISC_V_RS_HPP
