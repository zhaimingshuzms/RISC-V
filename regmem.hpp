//
// Created by Administrator on 2021/6/29.
//

#ifndef RISC_V_REGMEM_HPP
#define RISC_V_REGMEM_HPP
#include <cassert>
typedef unsigned int UINT;
class regmem{
public:
    UINT c[32];
    UINT x0shadow;
    regmem(){
        memset(c,0,sizeof(c));
    }
    UINT &operator[](const UINT &id){
        return id?c[id]:(x0shadow=0);
    }
};
#endif //RISC_V_REGMEM_HPP
