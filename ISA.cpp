//
// Created by Administrator on 2021/6/30.
//
#include "ISA.h"
#include "command.hpp"
#include "lib/myal.hpp"
input_traits::input_traits(const STR &s) {
    val=strtonum(s,16);
}
UINT input_traits::getval(){
    return val;
}
void ISA::read(){
    STR s;
    while (std::cin>>s){
        if (s[0]=='@') mem->seek(input_traits(s.substr(1,s.size()-1)).getval());
        else mem->write(input_traits(s).getval());
    }
}
void ISA::run(){
    mem->seek(0);
    int ttt=0;
    while (true){
        ++ttt;
        auto t=mem->get4(mem->pos);
        if (t==0x0ff00513){
            std::cout<<std::dec<<((reg->c[10])&255u);
            break;
        }
        auto past=mem->pos;
        command_base tmp(t);
        //std::cout<<"T :"<<std::hex<<t<<" "<<mem->get4(0x1084)<<std::endl;
        if (tmp.t==command_base::U) Ucommand(t).run(this);
        else if (tmp.t==command_base::J) Jcommand(t).run(this);
        else if (tmp.t==command_base::I) Icommand(t).run(this);
        else if (tmp.t==command_base::B) Bcommand(t).run(this);
        else if (tmp.t==command_base::S) Scommand(t).run(this);
        else if (tmp.t==command_base::R) Rcommand(t).run(this);
        else{
            std::cout<<"fuckyou!"<<std::endl;
            exit(0);
        }
        //ÌØÅÐjalr
        if (mem->pos==past) mem->pos+=4;
        //if (ttt%1000000==0) printf("order %d : ",ttt);
        //for (int i=0; i<32; ++i) printf("%u ",reg->c[i]);
        //std::cout<<"pc : "<<std::hex<<" "<<mem->pos;
        //printf("\n");
        //if (ttt>=1693) break;
    }
}
