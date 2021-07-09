//
// Created by Administrator on 2021/7/5.
//

#ifndef RISC_V_EX_HPP
#define RISC_V_EX_HPP
#include "basic_type.hpp"
#include "../command.hpp"
extern RSmessage EX_to_rob;
void init_run_ex(RSmessage rs){
    EX_to_rob = rs;
    EX_to_rob_pc = UINT_MAX;
    if (rs.c.t == command_base::U) Ucommand(rs.c).run(rs);
    else if (rs.c.t == command_base::R) Rcommand(rs.c).run(rs);
}
#endif //RISC_V_EX_HPP
