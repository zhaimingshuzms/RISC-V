//
// Created by Administrator on 2021/7/5.
//

#ifndef RISC_V_SIMULATOR_HPP
#define RISC_V_SIMULATOR_HPP
#include "pipeline/EX.hpp"
#include "pipeline/rob.h"
#include "pipeline/RS.hpp"
#include "pipeline/slbuffer.hpp"
#include "pipeline/inst_fetch_queue.hpp"
UINT commitmessage_d=32;
UINT commitmessage_v;
bool jump_flag;
UINT pc_jump;
UINT SLB_to_rob_d;
UINT SLB_to_rob_v;
void clear(){
    for (UINT i=0; i<8; ++i) rs[i].clear();
    ROB.clear();
}
void update(){
    ROB.tick();
    for (UINT i=0; i<8; ++i) rs[i].tick();
    slb.tick();

    if (jump_flag) {
        jump_flag = false;
        M.pos = pc_jump;
    }
}
void get_rob_pos(){
    ROB.in[ROB.tail]=issue_to_rob;
    ROB.pco[ROB.tail]=issue_to_rob_pc;
    ROB.pc[ROB.tail]=issue_to_rob_pc;
    if (issue_to_rob.t!=command_base::J&&issue_to_rob.t!=command_base::B) ROB.dest[ROB.tail]=issue_to_rob.rd;
    if (issue_to_rob.t==command_base::S) ROB.value[ROB.tail]=SLB_to_rob_v;
    if (issue_to_rob.opcode==0b0000011) ROB.dest[ROB.tail]=SLB_to_rob_d,ROB.value[ROB.tail]=SLB_to_rob_v;
    ROB.tail=ROB.next(ROB.tail);
}
void commit(){
    //pan kong
    if (ROB.ready[ROB.head]) {
        //???
        if (ROB.in[ROB.head].prev.t==command_base::S){
            if (ROB.in[ROB.head].prev.opcode2==0b000) M[ROB.dest[ROB.head]]=ROB.value[ROB.head];
            else if (ROB.in[ROB.head].prev.opcode2==0b000) M.get2(ROB.dest[ROB.head])=ROB.value[ROB.head];
            else M.get4(ROB.dest[ROB.head])=ROB.value[ROB.head];
        }
        if (ROB.in[ROB.head].prev.t == command_base::J || ROB.in[ROB.head].prev.t == command_base::B || ROB.in[ROB.head].prev.opcode == 0b1100111) {
            if (ROB.pc[ROB.head]!=ROB.pco[ROB.head]){
                jump_flag=true;
                pc_jump = ROB.pc[ROB.head];
            }
        }
        if (ROB.in[ROB.head].prev.t!=command_base::S&&ROB.in[ROB.head].prev.t != command_base::B) {
            commitmessage_d = ROB.dest[ROB.head];
            commitmessage_v = ROB.value[ROB.head];
        }
        //reg[ROB.dest[ROB.head]] = ROB.value[ROB.head];// a lot bug
        ROB.head = ROB.next(ROB.head);
    }
}
void run_rob(){
    //EX he slbuffer 改rob需要通過消息 commit 消息
    get_rob_pos();
    //EX
    if (EX_to_rob.c.t!=command_base::B) ROB.value[EX_to_rob.robid]=EX_to_rob_v;
    if (EX_to_rob_pc!=UINT_MAX) ROB.pc[EX_to_rob.robid]=EX_to_rob_pc;
    //SLBUFFER
}
void run_commit(){
    commit();
    if (jump_flag) clear();
}
void run_inst_fetch_queue(){
    q.push_command();
    auto tmp=q.get_command();
    if (!q.empty()) if_to_issue=tmp.first,if_to_issue_pc=tmp.second; else if_to_issue=command_base(0);
}
void run_reservation(){
    for (UINT i=0; i<8; ++i)
        if (rs[i].busy==false){
            if (!q.empty()) rs[i].push(issue_to_rs,issue_to_rs_pc);//dei cong issue shou
            break;
        }
}
void run_ex(){
    for (UINT i=0; i<8; ++i)
        if (rs[i].available()){
            init_run_ex(rs[i].out());
            break;
        }
}
void run_regfile(){
    if (commitmessage_d!=32) {
        reg.Q[commitmessage_d] = ROBSIZE;
        reg.V[commitmessage_d] = commitmessage_v;
        commitmessage_d=32;
    }
    if (issue_to_reg_d!=32){
        reg.Q[issue_to_reg_d]=issue_to_reg_v;
        issue_to_reg_d=32;
    }
}
void run_issue(){
    if (if_to_issue.opcode==0) return;
    if (ROB.full()) return;//wei kao lv
    if (if_to_issue.t==command_base::S||if_to_issue.opcode==0b0000011) {
        issue_to_reg_d=if_to_issue.rd;
        issue_to_reg_v=ROB.tail;
        issue_to_slb=if_to_issue;
        issue_to_rob=if_to_issue;
        return;
    }
    if (if_to_issue.t!=command_base::B){
        issue_to_reg_d=if_to_issue.rd;
        issue_to_reg_v=ROB.tail;
        issue_to_rob=if_to_issue;
        issue_to_rob_pc=if_to_issue_pc;
        issue_to_rs=if_to_issue;
        issue_to_rs_pc=if_to_issue_pc;
    }
}
void run_slbuffer(){

};
void run(){
    while (true){
        run_rob();
        if (code_from_rob_to_commit==0x0ff00513){
            std::cout<<std::dec<<((UINT)reg.V[10]&255u);
            break;
        }
        run_slbuffer();
        run_reservation();
        run_regfile();
        run_inst_fetch_queue();
        update();

        run_ex();
        run_issue();
        run_commit();//commit bu neng zhi jie gei ,yao fa gei run_regfile gai
    }
}

#endif //RISC_V_SIMULATOR_HPP
