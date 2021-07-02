//
// Created by Administrator on 2021/6/30.
//

#ifndef RISC_V_NUMAL_HPP
#define RISC_V_NUMAL_HPP
#include "sign.h"
static SINT sext(const UINT &u){
    return static_cast<SINT>(u);
}
static SCHAR sext(const UCHAR &u){
    return static_cast<SCHAR>(u);
}
static SSHORT sext(const USHORT &u){
    return static_cast<SSHORT>(u);
}
#endif //RISC_V_NUMAL_HPP
