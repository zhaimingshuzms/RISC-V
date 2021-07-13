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
#include "lib/myal.hpp"
UINT commitmessage_robid;
UINT commitmessage_d=32;
UINT commitmessage_v;
bool jump_flag;
UINT pc_jump;
UINT SLB_to_rob_d;
UINT SLB_to_rob_v;
RSmessage SLB_to_rob;
bool RSfull;
bool issuestall;
void clear(){
    //std::cerr<<"-----------------------------clear"<<std::endl;
    issue_to_rob=0;
    issue_to_slb=0;
    issue_to_rs=0;
    issue_to_reg_d=ROBSIZE;
    EX_to_rob.c=0;
    SLB_to_rob.c=0;
    q.Clear();
    for (UINT i=0; i<32; ++i) reg.Q(i)=ROBSIZE;
    for (UINT i=0; i<8; ++i) rs[i].clear();
    slb.q.clear();
    ROB.clear();
}
void update(){
    ROB.tick();
    for (UINT i=0; i<8; ++i) rs[i].tick();
    slb.tick();
}
void get_rob_pos(){
    //std::cerr<<"Get rob pos"<<issue_to_rob.origin_code<<" "<<issue_to_rob_pc<<std::endl;
    ROB.ready[ROB.tail]=false;//还没好
    ROB.in[ROB.tail]=issue_to_rob;
    ROB.pco[ROB.tail]=issue_to_rob_pc;
    ROB.pc[ROB.tail]=issue_to_rob_pc;
    //RS PART
    if (issue_to_rob.t!=command_base::B) ROB.dest[ROB.tail]=issue_to_rob.rd;
    //SLB PART
    if (issue_to_rob.t==command_base::S) ROB.value[ROB.tail]=SLB_to_rob_v;
    if (issue_to_rob.opcode==0b0000011) ROB.dest[ROB.tail]=SLB_to_rob_d,ROB.value[ROB.tail]=SLB_to_rob_v;
    ROB.tail=ROB.next(ROB.tail);
}
void commit(){
    //pan kong
    if (ROB.ready[ROB.head]) {
        std::cerr<<"ROB commit"<<ROB.head<<" "<<ROB.pc[ROB.head]<<" "<<ROB.in[ROB.head].prev.origin_code<<" "<<
        ROB.dest[ROB.head]<<" "<<ROB.value[ROB.head]<<std::endl;
        //???
        if (ROB.in[ROB.head].prev.t==command_base::S){
            if (ROB.in[ROB.head].prev.opcode2==0b000) M[ROB.dest[ROB.head]]=ROB.value[ROB.head];
            else if (ROB.in[ROB.head].prev.opcode2==0b001) M.get2(ROB.dest[ROB.head])=ROB.value[ROB.head];
            else{
                //std::cerr<<"SW"<<std::endl;
                M.get4(ROB.dest[ROB.head])=ROB.value[ROB.head];
            }
        }
        if (ROB.in[ROB.head].prev.t == command_base::J || ROB.in[ROB.head].prev.t == command_base::B || ROB.in[ROB.head].prev.opcode == 0b1100111) {
            if (ROB.pc[ROB.head]!=ROB.pco[ROB.head]){
                jump_flag=true;
                pc_jump = ROB.pc[ROB.head];
            }
        }
        if (ROB.in[ROB.head].prev.t!=command_base::S&&ROB.in[ROB.head].prev.t != command_base::B) {//有寄存器读写
            commitmessage_robid=ROB.head;
            commitmessage_d = ROB.dest[ROB.head];
            commitmessage_v = ROB.value[ROB.head];
            std::cerr<<"--------commitmessage"<<ROB.dest[ROB.head]<<" "<<ROB.value[ROB.head]<<std::endl;
        }
        //reg[ROB.dest[ROB.head]] = ROB.value[ROB.head];// a lot bug
        code_from_rob_to_commit=ROB.in[ROB.head].prev.origin_code;
        ROB.head = ROB.next(ROB.head);
    }
}
void run_rob(){
    //EX he slbuffer 改rob需要通過消息 commit 消息
    if (issue_to_rob.opcode!=0) get_rob_pos();//rob 满
    //EX
    //std::cerr<<"EX_to_rob"<<EX_to_rob.c.origin_code<<" "<<SLB_to_rob.c.origin_code<<" "<<issue_to_rob_pc<<std::endl;
    if (EX_to_rob.c.opcode&&EX_to_rob.c.t!=command_base::B){
        //std::cerr<<"EX -> rob modify value"<<EX_to_rob.robid<<" "<<EX_to_rob_v<<std::endl;
        ROB.value[EX_to_rob.robid]=EX_to_rob_v;
        ROB.ready[EX_to_rob.robid]=true;
    }
    if (EX_to_rob_pc!=UINT_MAX){
        //std::cerr<<"EX -> rob modify pc"<<EX_to_rob.robid<<" "<<EX_to_rob_pc<<std::endl;
        ROB.pc[EX_to_rob.robid]=EX_to_rob_pc;
        ROB.ready[EX_to_rob.robid]=true;
    }
    EX_to_rob.c=0;
    EX_to_rob_pc=UINT_MAX;
    //SLBUFFER
    if (SLB_to_rob.c.opcode) {
        //std::cerr<<"SLB_to_rob"<<SLB_to_rob.c.origin_code<<" "<<SLB_to_rob_d<<" "<<SLB_to_rob_v<<" "<<reg[9]<<std::endl;
        if (SLB_to_rob.c.t == command_base::S) ROB.dest[SLB_to_rob.robid] = SLB_to_rob_d; else ROB.dest[SLB_to_rob.robid]=SLB_to_rob.c.rd;
        ROB.value[SLB_to_rob.robid] = SLB_to_rob_v;
        ROB.ready[SLB_to_rob.robid]=true;
    }
    SLB_to_rob.c=0;
}
void run_commit(){
    commit();
    if (jump_flag){
        M.pos=pc_jump;
        clear();
        jump_flag=false;
    }
}
void run_inst_fetch_queue(){
    q.push_command();
    //std::cerr<<"IF queue size after push: "<<q.head<<" "<<q.tail<<" "<<M.pos<<std::endl;
    //std::cerr<<"empty ? "<<q.empty()<<std::endl;
    //std::cerr<<"RSfull"<<RSfull<<" "<<issuestall<<std::endl;
    if (issuestall) return;
    if (!q.empty()&&!RSfull){
        auto tmp=q.get_command();
        if_to_issue=tmp.first,if_to_issue_pc=tmp.second;
        //std::cerr<<"IF inst"<<tmp.first.origin_code<<" "<<tmp.second<<std::endl;
    }
    else if_to_issue=command_base(0);
}
void run_reservation(){
    for (UINT i=0; i<8; ++i) rs[i].flush();
    if (issue_to_rs.opcode==0){
        //std::cerr<<"rs nothing"<<std::endl;
        return;
    }
    bool find=false;
    for (UINT i=0; i<8; ++i)
        if (!rs[i].busy){
            //if (!q.empty()){
            //std::cerr<<"push RS "<<" "<<issue_to_rs.origin_code<<" "<<i<<" "<<issue_to_rs_pc<<std::endl;
            rs[i].push(issue_to_rs,issue_to_rs_pc);//dei cong issue shou
            //}
            find=true;
            break;
        }
    if (!find) RSfull=true;
    //assert(find);
    //rs 满
}
void run_ex(){
    EX_to_rob_pc=UINT_MAX;
    for (UINT i=0; i<8; ++i)
        if (rs[i].available()){
            //std::cerr<<"RSOUT"<<i<<" "<<rs[i].c.prev.origin_code<<std::endl;
            init_run_ex(rs[i].out());
            RSfull=false;
            break;
        }
    //ex 没东西
}
void run_regfile(){
    std::cerr<<"RUN regfile"<<commitmessage_d<<" "<<commitmessage_v<<" "<<issue_to_reg_d<<" "<<issue_to_reg_v<<std::endl;
    if (commitmessage_d!=32) {
        //std::cerr<<"regfile-------------"<<reg.Q[commitmessage_d]<<std::endl;
        if (commitmessage_robid==reg.Q(commitmessage_d)) reg.Q(commitmessage_d) = ROBSIZE;//最后一次
        reg[commitmessage_d] = commitmessage_v;
        commitmessage_d=32;
    }
    if (issue_to_reg_d!=32){
        reg.Q(issue_to_reg_d)=issue_to_reg_v;
        issue_to_reg_d=32;
    }
}
void run_issue(){
    std::cerr<<"if_to_issue"<<if_to_issue.origin_code<<" "<<if_to_issue_pc<<" "<<issuestall<<std::endl;
    if (if_to_issue.opcode==0) return;
    if (ROB.full()){
        assert(0);
        return;//wei kao lv
    }
    issue_to_rob=0;
    issue_to_slb=0;
    issue_to_rs=0;
    if (if_to_issue.t!=command_base::B&&reg.Q(if_to_issue.rd)!=ROBSIZE){//maybe not 0 寄存器
        std::cerr<<"stall"<<if_to_issue.rd<<" "<<reg.Q(if_to_issue.rd)<<" "<<if_to_issue.origin_code<<std::endl;
        issuestall=true;
        return;
    }
    issuestall=false;
    //std::cerr<<"true issue"<<std::endl;
    if (if_to_issue.t==command_base::S||if_to_issue.opcode==0b0000011) {
        if (if_to_issue.opcode==0b0000011) {
            issue_to_reg_d = if_to_issue.rd;
            issue_to_reg_v = ROB.tail;
        }
        issue_to_slb=if_to_issue;
        issue_to_slb_robid=ROB.tail;
        issue_to_rob=if_to_issue;
        issue_to_rob_pc=if_to_issue_pc;
        std::cerr<<"issue_to_reg_v"<<issue_to_reg_v<<" "<<issue_to_reg_d<<std::endl;
        return;
    }
    if (if_to_issue.t!=command_base::B) {
        issue_to_reg_d = if_to_issue.rd;
        issue_to_reg_v = ROB.tail;
    }
    std::cerr<<"issue_to_reg_v"<<issue_to_reg_v<<" "<<issue_to_reg_d<<std::endl;
    issue_to_rob=if_to_issue;
    issue_to_rob_pc=if_to_issue_pc;
    issue_to_rs=if_to_issue;
    issue_to_rs_robid=ROB.tail;
    issue_to_rs_pc=if_to_issue_pc;
}
void run_slbuffer(){
    slb.flush();
    if (issue_to_slb.opcode!=0) slb.push();
    if (!slb.q.empty()) slb.run();
}
class input_traits{
    UINT val;
public:
    explicit input_traits(const STR &s){
        val=strtonum(s,16);
    }
    UINT getval() const{
        return val;
    }
};
void scan(){
    STR s;
    while (std::cin>>s){
        if (s[0]=='@') M.seek(input_traits(s.substr(1,s.size()-1)).getval());
        else M.write(input_traits(s).getval());
    }
}
void run(){
    std::cerr<<std::hex;
    M.seek(0);
    int cycle=0;
    int la=0;
    while (true){
        ++cycle;
        std::cerr<<"cycle : "<<cycle<<" begin"<<" pos : "<<M.pos<<" "<<std::endl;
        //if (M[0x11b4]!=la) std::cerr<<"-------------change : "<<(UINT)M[0x11b4]<<std::endl;
        //la=M[0x11b4];
        run_rob();
        if (code_from_rob_to_commit==0x0ff00513){
            std::cout<<std::dec<<((UINT)reg[10]&255u);
            break;
        }

        run_slbuffer();
        run_reservation();
        run_regfile();

        run_inst_fetch_queue();

        update();
        std::cerr<<"rob ready"<<ROB.ready[5]<<std::endl;
        run_ex();
        run_issue();
        run_commit();//commit bu neng zhi jie gei ,yao fa gei run_regfile gai
        //for (int i=1; i<=3; ++i)
        //    std::cerr<<std::endl;
        if (cycle==1000) break;
        //if (reg[10]==3) break;
    }
}

#endif //RISC_V_SIMULATOR_HPP
