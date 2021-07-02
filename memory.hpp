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
    UCHAR *pool;
    memory():pool(new UCHAR [SIZE]()),pos(0){

    }
    void seek(UINT _pos){
        pos=_pos;
    }
    void write(UCHAR val){
        pool[pos++]=val;
    }
    UCHAR & operator [](const UINT &_pos){
        return pool[_pos];
    }
    USHORT & get2(const UINT &_pos){
        return *reinterpret_cast<USHORT*>(pool+_pos);
    }
    UINT &get4(const UINT &_pos){
        return *reinterpret_cast<UINT*>(pool+_pos);
    }
    void print_all(){
        for (UINT i=0; i<SIZE; ++i)
            if (pool[i]) std::cout<<i<<" "<<std::hex<<(UINT)pool[i]<<std::endl;
    }
};
#endif //RISC_V_MEMORY_HPP
