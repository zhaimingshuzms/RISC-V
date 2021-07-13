//
// Created by Administrator on 2021/7/2.
//

#ifndef RISC_V_MYQUEUE_HPP
#define RISC_V_MYQUEUE_HPP
#include "sign.h"
template<class T,int SIZE>
class myqueue{
public:
    UINT head,tail;
    T c[SIZE];
    myqueue(){
        head=0;
        tail=0;
        memset(c,0,sizeof(c));
    }
    void clear(){//weak clear
        head=tail=0;
    }
    static UINT next(UINT x){
        return (x+1)%SIZE;
    }
    bool empty(){
        return head==tail;
    }
    bool full(){
        return next(tail)==head;
    }
    T& operator [](const unsigned int &pos){
        return c[pos];
    }
    bool push(const T &x){
        if (full()) return false;
        c[tail]=x;
        tail=next(tail);
        return true;
    }
    T & front(){
        return c[head];
    }
    bool pop(){
        if (empty()) return false;
        head=next(head);
        return true;
    }
};
#endif //RISC_V_MYQUEUE_HPP
