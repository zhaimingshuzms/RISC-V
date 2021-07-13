//
// Created by Administrator on 2021/6/29.
//

#ifndef RISC_V_REGMEM_HPP
#define RISC_V_REGMEM_HPP
#include <cassert>
UINT issue_to_reg_d=32;
UINT issue_to_reg_v;
typedef unsigned int UINT;
class regmem{
    UINT V[32];
    UINT q[32];
public:
    UINT V0shadow;
    UINT q0shadow;
    regmem(){
        for (UINT i=0; i<32; ++i) q[i]=32;//ROBSIZE
        memset(V,0,sizeof(V));
    }
    UINT &operator[](const UINT &id){
        return id?V[id]:(V0shadow=0);
    }
    UINT &Q(const UINT &id){
        return id?q[id]:(q0shadow=32);
    }
}reg;
#endif //RISC_V_REGMEM_HPP
