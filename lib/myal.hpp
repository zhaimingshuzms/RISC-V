//
// Created by Administrator on 2021/6/29.
//

#ifndef RISC_V_MYAL_HPP
#define RISC_V_MYAL_HPP
#include <cstring>
#include "sign.h"
UINT strtonum(const STR &s,const UINT base){
    UINT ret=0;
    for (auto i:s)
        if (i>='0'&&i<='9') ret=ret*base+i-'0';
        else ret=ret*base+i-'A'+10;
    return ret;
}
STR nospace(const STR &s){
    STR t;
    for (auto i:s)
        if (i!=' '&&i!='\r'&&i!='\n') t+=i;
    return t;
}
SINT original(const UINT &u){
    SINT ret;
    if ((u>>31)&1) ret=-(((~u)&((1u<<31)-1))+1);
    else ret=u;
    return ret;
}
UINT complement(const SINT &s){
    UINT ret;
    memcpy(&ret,&s,sizeof(SINT));
    return ret;
}
#endif //RISC_V_MYAL_HPP
