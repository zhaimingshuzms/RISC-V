//
// Created by Administrator on 2021/7/5.
//

#ifndef RISC_V_EX_HPP
#define RISC_V_EX_HPP
#include "basic_type.hpp"
#include "../command.hpp"
extern RSmessage EX_to_rob;
void init_run_ex(RSmessage rs){
    //std::cerr<<"EX"<<rs.c.origin_code<<std::endl;
    EX_to_rob = rs;
    EX_to_rob_pc = UINT_MAX;
    if (rs.c.t == command_base::U) Ucommand(rs.c.origin_code).run(rs);
    else if (rs.c.t==command_base::J) Jcommand(rs.c.origin_code).run(rs);
    else if (rs.c.t == command_base::I) Icommand(rs.c.origin_code).run(rs);
    else if (rs.c.t == command_base::R) Rcommand(rs.c.origin_code).run(rs);
    else if (rs.c.t == command_base::B) Bcommand(rs.c.origin_code).run(rs);
}
#endif //RISC_V_EX_HPP
