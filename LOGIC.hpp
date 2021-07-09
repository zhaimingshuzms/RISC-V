//
// Created by Administrator on 2021/6/29.
//

#ifndef RISC_V_LOGIC_HPP
#define RISC_V_LOGIC_HPP
#include "lib/numal.hpp"
#include "memory.hpp"
#include "regmem.hpp"
//not modified yet
typedef unsigned int UINT;
typedef int SINT;
extern memory M;
extern bool jump_flag;
namespace logic{
    void lui(UINT &rd,UINT &imm){
        //std::cout<<"lui"<<" "<<rd<<" "<<imm<<std::endl;
        rd=sext(imm);
    }
    void auipc(UINT &rd,UINT &pc_o,UINT &imm){
        rd=pc_o+sext(imm);
    }
    void jal(UINT &rd,UINT &offset,UINT &pc_o,UINT &pc_j){
        //std::cout<<"jal"<<std::dec<<rd<<" "<<sext(offset)<<" "<<pc<<std::endl;
        rd=pc_o+4;
        pc_j=pc_o+sext(offset);
    }
    void jalr(UINT &rs1,UINT &rd,UINT &offset,UINT &pc_o,UINT &pc_j){
        //std::cout<<"jalr "<<std::dec<<rs1<<" "<<rd<<" "<<offset<<" "<<pc<<std::endl;
        auto t=pc_o+4;
        pc_j=(rs1+sext(offset))&(~1u);
        rd=t;
        //std::cout<<"nowpc"<<pc<<std::endl;
    }
    void beq(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc_o,UINT &pc_j){
        if (rs1==rs2) pc_j=pc_o+sext(offset);
    }
    void bne(UINT &rs1,UINT &rs2,UINT &offset,UINT pc_o,UINT &pc_j){
        if (rs1!=rs2) pc_j=pc_o+sext(offset);
    }
    void blt(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc_o,UINT &pc_j){
        if (sext(rs1)<sext(rs2)) pc_j=pc_o+sext(offset);
    }
    void bge(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc_o,UINT &pc_j){
        if (sext(rs1)>=sext(rs2)) pc_j=pc_o+sext(offset);
    }
    void bltu(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc_o,UINT &pc_j){
        if (rs1<rs2) pc_j=pc_o+sext(offset);
    }
    void bgeu(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc_o,UINT &pc_j){
        if (rs1>=rs2) pc_j=pc_o+sext(offset);
    }
    void lb(UINT &rs1,UINT &rd,UINT &offset){
        rd=sext(M[rs1+sext(offset)]);
    }
    void lbu(UINT &rs1,UINT &rd,UINT &offset){
        rd=M[rs1+sext(offset)];
    }
    void lh(UINT &rs1,UINT &rd,UINT &offset){
        rd=sext(M.get2(rs1+sext(offset)));
    }
    void lhu(UINT &rs1,UINT &rd,UINT &offset){
        rd=M.get2(rs1+sext(offset));
    }
    void lw(UINT &rs1,UINT &rd,UINT &offset){
        //std::cout<<"lw"<<std::dec<<x[rs1]+sext(offset)<<std::endl;
        rd=sext(M.get4(rs1+sext(offset)));
    }
    void lwu(UINT &rs1,UINT &rd,UINT &offset){
        rd=M.get4(rs1+sext(offset));
    }
    void sb(UINT &rs1,UINT &rs2,UINT &offset,UINT &d,UINT &v){
        d=rs1+sext(offset);
        v=(rs2&0xffu);
    }
    void sh(UINT &rs1,UINT &rs2,UINT &offset,UINT &d,UINT &v){
        d=rs1+sext(offset);
        v=(rs2&0xffffu);
    }
    void sw(UINT &rs1,UINT &rs2,UINT &offset,UINT &d,UINT &v){
        //std::cout<<"sw"<<std::hex<<" "<<x[rs1]<<" "<<x[rs1]+sext(offset)<<" "<<x[rs2]<<" "<<offset<<std::endl;
        d=rs1+sext(offset);
        v=rs2;
    }
    void addi(UINT &rs1,UINT &rd,UINT &imm){
        //std::cout<<"addi"<<rs1<<" "<<rd<<" "<<imm<<" "<<std::dec<<sext(imm)<<std::endl;
        rd=rs1+sext(imm);
    }
    void slti(UINT &rs1,UINT &rd,UINT &imm){
        rd=(sext(rs1)<sext(imm));
    }
    void sltiu(UINT &rs1,UINT &rd,UINT &imm){
        rd=(rs1<imm);
    }
    void xori(UINT &rs1,UINT &rd,UINT &imm){
        rd=rs1^sext(imm);
    }
    void ori(UINT &rs1,UINT &rd,UINT &imm){
        rd=rs1|sext(imm);
    }
    void andi(UINT &rs1,UINT &rd,UINT &imm){
        rd=rs1&sext(imm);
    }
    void slli(UINT &rs1,UINT &rd,UINT &imm){
        rd=rs1<<imm;
    }
    void srli(UINT &rs1,UINT &rd,UINT &imm){
        rd=rs1>>imm;
    }
    void srai(UINT &rs1,UINT &rd,UINT &imm){
        rd=sext(rs1)>>imm;
    }
};
#endif //RISC_V_LOGIC_HPP
