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
class LOGIC{
public:
    void lui(UINT &rd,UINT &imm,regmem &x){
        //std::cout<<"lui"<<" "<<rd<<" "<<imm<<std::endl;
        x[rd]=sext(imm);
    }
    void auipc(UINT &rd,UINT &imm,UINT &pc,regmem &x){
        x[rd]=pc+sext(imm);
    }
    void jal(UINT &rd,UINT &offset,UINT &pc,regmem &x){
        //std::cout<<"jal"<<std::dec<<rd<<" "<<sext(offset)<<" "<<pc<<std::endl;
        x[rd]=pc+4;
        pc+=sext(offset);
    }
    void jalr(UINT &rs1,UINT &rd,UINT &offset,UINT &pc,regmem &x){
        //std::cout<<"jalr "<<std::dec<<rs1<<" "<<rd<<" "<<offset<<" "<<pc<<std::endl;
        auto t=pc+4;
        pc=(x[rs1]+sext(offset))&(~1u);
        x[rd]=t;
        //std::cout<<"nowpc"<<pc<<std::endl;
    }
    void beq(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc,regmem &x){
        //std::cout<<"beq"<<std::dec<<rs1<<" "<<rs2<<" "<<pc<<std::endl;
        if (x[rs1]==x[rs2]) pc+=sext(offset);
        //std::cout<<"pc "<<pc<<std::endl;
    }
    void bne(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc,regmem &x){
        if (x[rs1]!=x[rs2]) pc+=sext(offset);
    }
    void blt(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc,regmem &x){
        if (sext(x[rs1])<sext(x[rs2])) pc+=sext(offset);
    }
    void bge(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc,regmem &x){
        if (sext(x[rs1])>=sext(x[rs2])) pc+=sext(offset);
    }
    void bltu(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc,regmem &x){
        if (x[rs1]<x[rs2]) pc+=sext(offset);
    }
    void bgeu(UINT &rs1,UINT &rs2,UINT &offset,UINT &pc,regmem &x){
        if (x[rs1]>=x[rs2]) pc+=sext(offset);
    }
    void lb(UINT &rs1,UINT &rd,UINT &offset,regmem &x,memory &M){
        x[rd]=sext(M[x[rs1]+sext(offset)]);
    }
    void lbu(UINT &rs1,UINT &rd,UINT &offset,regmem &x,memory &M){
        x[rd]=M[x[rs1]+sext(offset)];
    }
    void lh(UINT &rs1,UINT &rd,UINT &offset,regmem &x,memory &M){
        x[rd]=sext(M.get2(x[rs1]+sext(offset)));
    }
    void lhu(UINT &rs1,UINT &rd,UINT &offset,regmem &x,memory &M){
        x[rd]=M.get2(x[rs1]+sext(offset));
    }
    void lw(UINT &rs1,UINT &rd,UINT &offset,regmem &x,memory &M){
        //std::cout<<"lw"<<std::dec<<x[rs1]+sext(offset)<<std::endl;
        x[rd]=sext(M.get4(x[rs1]+sext(offset)));
    }
    void lwu(UINT &rs1,UINT &rd,UINT &offset,regmem &x,memory &M){
        x[rd]=M.get4(x[rs1]+sext(offset));
    }
    void sb(UINT &rs1,UINT &rs2,UINT &offset,regmem &x,memory &M){
        M[x[rs1]+sext(offset)]=(x[rs2]&0xffu);
    }
    void sh(UINT &rs1,UINT &rs2,UINT &offset,regmem &x,memory &M){
        M.get2(x[rs1]+sext(offset))=(x[rs2]&0xffffu);
    }
    void sw(UINT &rs1,UINT &rs2,UINT &offset,regmem &x,memory &M){
        //std::cout<<"sw"<<std::hex<<" "<<x[rs1]<<" "<<x[rs1]+sext(offset)<<" "<<x[rs2]<<" "<<offset<<std::endl;
        M.get4(x[rs1]+sext(offset))=x[rs2];
    }
    void addi(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        //std::cout<<"addi"<<rs1<<" "<<rd<<" "<<imm<<" "<<std::dec<<sext(imm)<<std::endl;
        x[rd]=x[rs1]+sext(imm);
    }
    void slti(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=(sext(x[rs1])<sext(imm));
    }
    void sltiu(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=(x[rs1]<imm);
    }
    void xori(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=x[rs1]^sext(imm);
    }
    void ori(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=x[rs1]|sext(imm);
    }
    void andi(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=x[rs1]&sext(imm);
    }
    void slli(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=x[rs1]<<imm;
    }
    void srli(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=x[rs1]>>imm;
    }
    void srai(UINT &rs1,UINT &rd,UINT &imm,regmem &x){
        x[rd]=sext(x[rs1])>>imm;
    }
};
#endif //RISC_V_LOGIC_HPP
