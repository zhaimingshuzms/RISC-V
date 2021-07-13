//
// Created by Administrator on 2021/6/29.
//

#ifndef RISC_V_MEMORY_HPP
#define RISC_V_MEMORY_HPP
#include <iomanip>
#include "lib/sign.h"
const int SIZE=500001;
class memory{
public:
    UINT pos;
    UCHAR pool[SIZE];
    UCHAR tmp1;
    USHORT tmp2;
    UINT tmp3;
    memory():pos(0){

    }
    void seek(UINT _pos){
        pos=_pos;
    }
    void write(UCHAR val){
        pool[pos++]=val;
    }
    UCHAR & operator [](const UINT &_pos){
        if (_pos>=SIZE) return tmp1;//load ÔÚ jump ºó£¬µØÖ·´íÎó
        return pool[_pos];
    }
    USHORT & get2(const UINT &_pos){
        if (_pos>=SIZE) return tmp2;
        return *reinterpret_cast<USHORT*>(pool+_pos);
    }
    UINT &get4(const UINT &_pos){
        if (_pos>=SIZE) return tmp3;
        return *reinterpret_cast<UINT*>(pool+_pos);
    }
    void print_all(){
        for (UINT i=0; i<SIZE; ++i)
            if (pool[i]) std::cout<<i<<" "<<std::hex<<(UINT)pool[i]<<std::endl;
    }
}M;
#endif //RISC_V_MEMORY_HPP
