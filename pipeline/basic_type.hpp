//
// Created by Administrator on 2021/7/8.
//

#ifndef RISC_V_BASIC_TYPE_HPP
#define RISC_V_BASIC_TYPE_HPP
#include "../lib/sign.h"
class command_base{
public:
    enum type{
        U,J,I,B,S,R
    }t;
    UINT opcode,opcode2,opcode3;
    UINT rd,rs1,rs2;
    UINT imm;
    UINT getval(const UINT &u,const UINT &lo,const UINT &hi){
        return (u>>lo)&((1u<<(hi-lo+1))-1u);
    }
//²¹ÂëÀ©Õ¹Î»Êý
    UINT extrabit(const UINT &u,const UINT lo){
        return u?0xffffffffu <<lo:0;
    }
    command_base(const UINT &in=0){
        imm=4294967295u;//care
        opcode=getval(in,0,6);
        opcode2=getval(in,12,14);
        opcode3=getval(in,25,31);
        rd=getval(in,7,11);
        rs1=getval(in,15,19);
        rs2=getval(in,20,24);
        if (opcode==0b0110111||opcode==0b0010111) t=U;
        else if (opcode==0b1101111) t=J;
        else if (opcode==0b1100111||opcode==0b0000011||opcode==0b0010011) t=I;
        else if (opcode==0b1100011) t=B;
        else if (opcode==0b0100011) t=S;
        else t=R;
    }
};
class RSmessage{
public:
    command_base c;
    UINT V1,V2;
    UINT robid;
    UINT pc;
    RSmessage():c(0){
    }
    RSmessage(const command_base &_c,const UINT &V1,const UINT &V2,const UINT &_robid,const UINT &pc):c(_c),V1(V1),V2(V2),robid(_robid),pc(pc){
    }
};
#endif //RISC_V_BASIC_TYPE_HPP
