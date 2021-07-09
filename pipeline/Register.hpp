//
// Created by Administrator on 2021/7/2.
//

#ifndef RISC_V_REGISTER_HPP
#define RISC_V_REGISTER_HPP
//from https://github.com/skyzh/RISCV-Simulator/blob/out-of-order/src/Common/Register.hpp
template<class T>
class Register{
public:
    T prev,next;
    bool _stall;
    Register():prev(0),next(0),_stall(false){}
    Register(T d):prev(d),next(d),_stall(false){}
    void tick(){
        if (!_stall) prev=next;
    }
    operator T(){
        return prev;
    }
    void operator =(const T &x){
        next=x;
    }
};
#endif //RISC_V_REGISTER_HPP
