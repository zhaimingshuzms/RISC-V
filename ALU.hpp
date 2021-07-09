//
// Created by Administrator on 2021/6/29.
//

#ifndef RISC_V_ALU_HPP
#define RISC_V_ALU_HPP
#include "lib/numal.hpp"
//ALU 是static，可能带来一些问题,static 函数是有地址的
namespace ALU{
    static void add(UINT &rs1,UINT &rs2,UINT &rd){
        rd=rs1+rs2;
    }
    static void sub(UINT &rs1,UINT &rs2,UINT &rd){
        rd=rs1-rs2;
    }
    static void sll(UINT &rs1,UINT &rs2,UINT &rd){
        rd=rs1<<(rs2&((1u<<5)-1));
    }
    static void slt(UINT &rs1,UINT &rs2,UINT &rd){
        rd=(sext(rs1)<sext(rs2));
    }
    static void sltu(UINT &rs1,UINT &rs2,UINT &rd){
        rd=(rs1<rs2);
    }
    static void Xor(UINT &rs1,UINT &rs2,UINT &rd){
        //std::cout<<"__xor"<<rs1<<" "<<rs2<<" "<<rd<<" "<<rs1<<" "<<rs2<<std::endl;
        rd=rs1^rs2;
        //std::cout<<"result: "<<rd<<std::endl;
    }
    static void srl(UINT &rs1,UINT &rs2,UINT &rd){
        rd=rs1>>(rs2&((1u<<5)-1));
    }
    static void sra(UINT &rs1,UINT &rs2,UINT &rd){
        rd=(sext(rs1)>>(rs2&((1u<<5)-1)));
    }
    static void Or(UINT &rs1,UINT &rs2,UINT &rd){
        rd=rs1|rs2;
    }
    static void And(UINT &rs1,UINT &rs2,UINT &rd){
        rd=rs1&rs2;
    }
};
#endif //RISC_V_ALU_HPP
