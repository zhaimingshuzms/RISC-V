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
public:
    UINT Q[32];
    UINT V[32];
    UINT x0shadow;
    regmem(){
        memset(Q,0,sizeof(Q));
        memset(V,0,sizeof(V));
    }
    /*UINT &operator[](const UINT &id){
        return id?c[id]:(x0shadow=0);
    }*/
}reg;
#endif //RISC_V_REGMEM_HPP
