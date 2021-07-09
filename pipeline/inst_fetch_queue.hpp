//
// Created by Administrator on 2021/7/2.
//

#ifndef RISC_V_INST_FETCH_QUEUE_HPP
#define RISC_V_INST_FETCH_QUEUE_HPP
static const int FETCHQUEUESIZE=32;
#include <iostream>
#include "../command.hpp"
#include "../lib/myqueue.hpp"
command_base if_to_issue(0);
UINT if_to_issue_pc;
class inst_fetch_queue:public myqueue<command_base,FETCHQUEUESIZE>{
public:
    myqueue<UINT,FETCHQUEUESIZE> pco;
    bool push_command(){
        while (!full()){
            auto t=M.get4(M.pos);
            if (t==0x0ff00513) return false;
            auto tmp=command_base(t);
            if (tmp.t==command_base::U) tmp=Ucommand(t);
            else if (tmp.t==command_base::J) tmp=Jcommand(t);
            else if (tmp.t==command_base::I) tmp=Icommand(t);
            else if (tmp.t==command_base::B) tmp=Bcommand(t);
            else if (tmp.t==command_base::S) tmp=Scommand(t);
            else if (tmp.t==command_base::R) tmp=Rcommand(t);
            else{
                std::cout<<"fuckyou!"<<std::endl;
                exit(0);
            }
            push(t);
            pco.push(M.pos);
            M.pos+=4;
        }
        return true;
    }
    std::pair<command_base,UINT> get_command(){
        return std::make_pair(c[head++],pco[pco.head++]);
    }
}q;
#endif //RISC_V_INST_FETCH_QUEUE_HPP
