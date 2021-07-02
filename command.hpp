//
// Created by Administrator on 2021/6/29.
//
//not tested yet
#ifndef RISC_V_COMMAND_HPP
#define RISC_V_COMMAND_HPP
#include "ISA.h"
#include <tuple>
typedef unsigned int UINT;
//following https://github.com/skyzh/RISCV-Simulator/blob/seq/src/Instruction.hpp
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
//补码扩展位数
    UINT extrabit(const UINT &u,const UINT lo){
        return u?0xffffffffu <<lo:0;
    }
    command_base(const UINT &in){
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
class Ucommand:command_base{
    public:
    Ucommand(const UINT &in):command_base(in){
        //std::cout<<"U"<<std::endl;
        imm=getval(in,12,31)<<12;
    }
    void run(ISA *p){
        if (opcode==0b0110111) p->logic->lui(rd,imm,*p->reg);
        else p->logic->auipc(rd,imm,p->mem->pos,*p->reg);
    }
};
class Jcommand:command_base{
public:
    Jcommand(const UINT &in):command_base(in){
        //std::cout<<"J"<<std::endl;
        imm=(getval(in,12,19)<<12)|
            (getval(in,20,20)<<11)|
            (getval(in,21,30)<<1)|
            extrabit(getval(in,31,31),20);
    }
    void run(ISA *p){
        p->logic->jal(rd,imm,p->mem->pos,*p->reg);
    }
};
class Icommand:command_base{
public:
    Icommand(const UINT &in):command_base(in){
        //std::cout<<"I"<<std::endl;
        //std::cout<<std::hex<<in<<std::endl;
        imm=getval(in,20,30)|
            extrabit(getval(in,31,31),11);
        //std::cout<<std::hex<<getval(in,20,30)<<" "<<extrabit(getval(in,31,31),11)<<std::endl;
    }
    void run(ISA *p){
        if (opcode==0b1100111) p->logic->jalr(rs1,rd,imm,p->mem->pos,*p->reg);
        else if (opcode==0b0000011) {
            if (opcode2 == 0b000) p->logic->lb(rs1, rd, imm, *p->reg, *p->mem);
            else if (opcode2 == 0b001) p->logic->lh(rs1, rd, imm, *p->reg, *p->mem);
            else if (opcode2 == 0b010) p->logic->lw(rs1, rd, imm, *p->reg, *p->mem);
            else if (opcode2 == 0b100) p->logic->lbu(rs1, rd, imm, *p->reg, *p->mem);
            else if (opcode2 == 0b101) p->logic->lhu(rs1, rd, imm, *p->reg, *p->mem);
        }
        else{
            if (opcode2==0b000) p->logic->addi(rs1,rd,imm,*p->reg);
            else if (opcode2==0b010) p->logic->slti(rs1,rd,imm,*p->reg);
            else if (opcode2==0b011) p->logic->sltiu(rs1,rd,imm,*p->reg);
            else if (opcode2==0b100) p->logic->xori(rs1,rd,imm,*p->reg);
            else if (opcode2==0b110) p->logic->ori(rs1,rd,imm,*p->reg);
            else if (opcode2==0b111) p->logic->andi(rs1,rd,imm,*p->reg);
            else if (opcode2==0b001) p->logic->slli(rs1,rd,rs2,*p->reg);
            else if (opcode2==0b101){
                if (opcode3==0b0000000) p->logic->srli(rs1,rd,rs2,*p->reg);
                else p->logic->srai(rs1,rd,rs2,*p->reg);
            }
        }
    }
};
class Bcommand:command_base{
public:
    Bcommand(const UINT &in):command_base(in){
        //std::cout<<"B"<<std::endl;
        imm=(getval(in,7,7)<<11)|
            (getval(in,8,11)<<1)|
            (getval(in,25,30)<<5)|
            extrabit(getval(in,31,31),12);
    }
    void run(ISA *p){
        if (opcode2==0b000) p->logic->beq(rs1,rs2,imm,p->mem->pos,*p->reg);
        else if (opcode2==0b001) p->logic->bne(rs1,rs2,imm,p->mem->pos,*p->reg);
        else if (opcode2==0b100) p->logic->blt(rs1,rs2,imm,p->mem->pos,*p->reg);
        else if (opcode2==0b101) p->logic->bge(rs1,rs2,imm,p->mem->pos,*p->reg);
        else if (opcode2==0b110) p->logic->bltu(rs1,rs2,imm,p->mem->pos,*p->reg);
        else if (opcode2==0b111) p->logic->bgeu(rs1,rs2,imm,p->mem->pos,*p->reg);
    }
};
class Scommand:command_base{
public:
    Scommand(const UINT &in):command_base(in){
        //std::cout<<"S"<<std::endl;
        imm=getval(in,7,11)|
            (getval(in,25,30)<<5)|
            extrabit(getval(in,31,31),11);
    }
    void run(ISA *p){
        if (opcode2==0b000) p->logic->sb(rs1,rs2,imm,*p->reg,*p->mem);
        else if (opcode2==0b001) p->logic->sh(rs1,rs2,imm,*p->reg,*p->mem);
        else if (opcode2==0b010) p->logic->sw(rs1,rs2,imm,*p->reg,*p->mem);
    }
};
class Rcommand:command_base{
public:
    Rcommand(const UINT &in):command_base(in){
        //还是ALU好啊！
        //std::cout<<"R"<<std::endl;
    }
    void run(ISA *p){
        if (opcode2==0b000){
            if (opcode3==0b0000000) p->alu->add(rs1,rs2,rd,*p->reg);
            else p->alu->sub(rs1,rs2,rd,*p->reg);
        }
        else if (opcode2==0b001) p->alu->sll(rs1,rs2,rd,*p->reg);
        else if (opcode2==0b010) p->alu->slt(rs1,rs2,rd,*p->reg);
        else if (opcode2==0b011) p->alu->sltu(rs1,rs2,rd,*p->reg);
        else if (opcode2==0b100) p->alu->Xor(rs1,rs2,rd,*p->reg);
        else if (opcode2==0b101){
            if (opcode3==0b0000000) p->alu->srl(rs1,rs2,rd,*p->reg);
            else p->alu->sra(rs1,rs2,rd,*p->reg);
        }
        else if (opcode2==0b110) p->alu->Or(rs1,rs2,rd,*p->reg);
        else p->alu->And(rs1,rs2,rd,*p->reg);
    }
};
#endif //RISC_V_COMMAND_HPP
