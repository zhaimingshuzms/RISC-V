//
// Created by Administrator on 2021/6/29.
//
//not tested yet
#ifndef RISC_V_COMMAND_HPP
#define RISC_V_COMMAND_HPP
#include "pipeline/basic_type.hpp"
#include "LOGIC.hpp"
#include "ALU.hpp"
typedef unsigned int UINT;
//following https://github.com/skyzh/RISCV-Simulator/blob/seq/src/Instruction.hpp
extern UINT EX_to_rob_v;
extern UINT EX_to_rob_pc;
extern UINT SLB_to_rob_d;
extern UINT SLB_to_rob_v;
extern RSmessage SLB_to_rob;
struct Ucommand:command_base{
    public:
    Ucommand(const UINT &in):command_base(in){
        //std::cout<<"U"<<std::endl;
        imm=getval(in,12,31)<<12;
    }
    void run(RSmessage &rs){
        //std::cerr<<"Ucommand"<<imm<<std::endl;
        if (opcode==0b0110111) logic::lui(EX_to_rob_v,imm);
        else logic::auipc(EX_to_rob_v,rs.pc,imm);
    }
};
struct Jcommand:command_base{
public:
    Jcommand(const UINT &in):command_base(in){
        //std::cout<<"J"<<std::endl;
        imm=(getval(in,12,19)<<12)|
            (getval(in,20,20)<<11)|
            (getval(in,21,30)<<1)|
            extrabit(getval(in,31,31),20);
    }
    void run(RSmessage &rs){
        logic::jal(EX_to_rob_v,imm,rs.pc,EX_to_rob_pc);
    }
};
struct Icommand:command_base{
public:
    Icommand(const UINT &in):command_base(in){
        imm=getval(in,20,30)|
            extrabit(getval(in,31,31),11);
    }
    void run(RSmessage &rs){
        if (opcode==0b1100111) logic::jalr(rs.V1,EX_to_rob_v,imm,rs.pc,EX_to_rob_pc);
        else if (opcode==0b0000011) {
            SLB_to_rob=rs;
            if (opcode2 == 0b000) logic::lb(rs.V1, SLB_to_rob_v, imm);
            else if (opcode2 == 0b001) logic::lh(rs.V1, SLB_to_rob_v, imm);
            else if (opcode2 == 0b010) logic::lw(rs.V1, SLB_to_rob_v, imm);
            else if (opcode2 == 0b100) logic::lbu(rs.V1, SLB_to_rob_v, imm);
            else if (opcode2 == 0b101) logic::lhu(rs.V1, SLB_to_rob_v, imm);
        }
        else{
            //std::cerr<<"Icommand"<<rs.V1<<" "<<imm<<std::endl;
            if (opcode2==0b000) logic::addi(rs.V1,EX_to_rob_v,imm);
            else if (opcode2==0b010) logic::slti(rs.V1,EX_to_rob_v,imm);
            else if (opcode2==0b011) logic::sltiu(rs.V1,EX_to_rob_v,imm);
            else if (opcode2==0b100) logic::xori(rs.V1,EX_to_rob_v,imm);
            else if (opcode2==0b110) logic::ori(rs.V1,EX_to_rob_v,imm);
            else if (opcode2==0b111) logic::andi(rs.V1,EX_to_rob_v,imm);
            else if (opcode2==0b001) logic::slli(rs.V1,EX_to_rob_v,imm);
            else if (opcode2==0b101){
                if (opcode3==0b0000000) logic::srli(rs.V1,EX_to_rob_v,imm);
                else logic::srai(rs.V1,EX_to_rob_v,imm);
            }
        }
    }
};
struct Bcommand:command_base{
public:
    Bcommand(const UINT &in):command_base(in){
        //std::cout<<"B"<<std::endl;
        imm=(getval(in,7,7)<<11)|
            (getval(in,8,11)<<1)|
            (getval(in,25,30)<<5)|
            extrabit(getval(in,31,31),12);
    }
    void run(RSmessage &rs){
        //std::cerr<<"Borigin"<<rs1<<" "<<rs2<<std::endl;
        if (opcode2==0b000) logic::beq(rs.V1,rs.V2,imm,rs.pc,EX_to_rob_pc);
        else if (opcode2==0b001) logic::bne(rs.V1,rs.V2,imm,rs.pc,EX_to_rob_pc);
        else if (opcode2==0b100) logic::blt(rs.V1,rs.V2,imm,rs.pc,EX_to_rob_pc);
        else if (opcode2==0b101) logic::bge(rs.V1,rs.V2,imm,rs.pc,EX_to_rob_pc);
        else if (opcode2==0b110) logic::bltu(rs.V1,rs.V2,imm,rs.pc,EX_to_rob_pc);
        else if (opcode2==0b111) logic::bgeu(rs.V1,rs.V2,imm,rs.pc,EX_to_rob_pc);
        if (EX_to_rob_pc==UINT_MAX) EX_to_rob_pc=rs.pc;
    }
};
struct Scommand:command_base{
public:
    Scommand(const UINT &in):command_base(in){
        //std::cout<<"S"<<std::endl;
        imm=getval(in,7,11)|
            (getval(in,25,30)<<5)|
            extrabit(getval(in,31,31),11);
    }
    void run(RSmessage &rs){
        SLB_to_rob=rs;
        if (opcode2==0b000) logic::sb(rs.V1,rs.V2,imm,SLB_to_rob_d,SLB_to_rob_v);
        else if (opcode2==0b001) logic::sh(rs.V1,rs.V2,imm,SLB_to_rob_d,SLB_to_rob_v);
        else if (opcode2==0b010) logic::sw(rs.V1,rs.V2,imm,SLB_to_rob_d,SLB_to_rob_v);
    }
};
struct Rcommand:command_base{
public:
    Rcommand(const UINT &in):command_base(in){
        //????ALU??????
        //std::cout<<"R"<<std::endl;
    }
    void run(RSmessage &rs){
        if (opcode2==0b000){
            if (opcode3==0b0000000) ALU::add(rs.V1,rs.V2,EX_to_rob_v);
            else ALU::sub(rs.V1,rs.V2,EX_to_rob_v);
        }
        else if (opcode2==0b001) ALU::sll(rs.V1,rs.V2,EX_to_rob_v);
        else if (opcode2==0b010) ALU::slt(rs.V1,rs.V2,EX_to_rob_v);
        else if (opcode2==0b011) ALU::sltu(rs.V1,rs.V2,EX_to_rob_v);
        else if (opcode2==0b100) ALU::Xor(rs.V1,rs.V2,EX_to_rob_v);
        else if (opcode2==0b101){
            if (opcode3==0b0000000) ALU::srl(rs.V1,rs.V2,EX_to_rob_v);
            else ALU::sra(rs.V1,rs.V2,EX_to_rob_v);
        }
        else if (opcode2==0b110) ALU::Or(rs.V1,rs.V2,EX_to_rob_v);
        else ALU::And(rs.V1,rs.V2,EX_to_rob_v);
    }
};
#endif //RISC_V_COMMAND_HPP
