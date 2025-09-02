#include "Vserdiv__Syms.h"
#include "Vserdiv___024root.h"
#include "testbench.h"
#include "ticks.h"
#include <iostream>
int main(int argc, char **argv, char **env) {

  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(VM_TRACE);
#ifdef HAS_COVERAGE
  char *coveragepath;
  coveragepath = getenv("COVERAGEFILE");
#endif // HAS_COVERAGE

  ////////
  // Get the env vars early to avoid Verilator segfaults.
  ////////

  std::cout << "Starting getting env variables." << std::endl;


  ////////
  // Initialize testbench.
  ////////

  Testbench *tb = new Testbench(cl_get_tracefile());

  ////////
  // Run the testbench.
  ////////
  
  tb->module_->rst_ni = 0;
  tb->tick(1);
  tb->tick(1);
  tb->module_->rst_ni = 1;
  tb->tick(1);
  std::cout << "Reset done." << std::endl;
  tb->tick(1);
  while(tb->module_->in_rdy_o == 0) {
    tb->tick(1);
  }
  tb->module_->in_vld_i = 1;
  tb->module_->id_i = 0;
  tb->module_->op_a_i = 0x11;
  tb->module_->op_b_i = 0x11;
  tb->module_->op_a_i_t0 = 0xff;
  tb->module_->opcode_i = 0;
  tb->tick(1);
  tb->module_->in_vld_i = 0;

  uint32_t n_ticks = 0;
  while(tb->module_->out_vld_o != 1) {
    tb->tick(1);
    n_ticks++;
    if(tb->module_->out_vld_o_t0){
      std::cout << "tb->module_->out_vld_o is tainted" << std::endl;
    }
  }

  std::cout << "out_vld_o: " << (uint32_t) tb->module_->out_vld_o << std::endl;
  std::cout << "Result: 0x" << std::hex << tb->module_->res_o << " with taint " <<  std::hex << tb->module_->res_o_t0 << " after " << std::dec << n_ticks << " ticks." << std::endl;

  // for(int i = 0; i < 10; i++) {
  //   tb->tick(1);
  //   tb->module_->out_rdy_i = 1;
  // }
  // tb->module_->out_rdy_i = 0;


  // while(tb->module_->in_rdy_o == 0) {
  //   tb->tick(1);
  //   std::cout << "ct_state_q" << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__ct_state_q << std::endl;

  // }
  // tb->tick(1);
  // tb->tick(1);
  // tb->tick(1);

  // tb->module_->in_vld_i = 1;
  // tb->module_->id_i = 1;
  // tb->module_->op_a_i = 0x1000;
  // tb->module_->op_b_i = 0x1;
  // tb->module_->opcode_i = 0;
  // std::cout << "ct_state_q " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__ct_state_q << std::endl;
  // std::cout << "state_q " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__state_q << std::endl;
  // std::cout << "cnt_q_buf " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__cnt_buf_q << std::endl;

  // tb->tick(1);
  // tb->module_->in_vld_i = 0;
  // std::cout << "ct_state_q " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__ct_state_q << std::endl;
  // std::cout << "state_q " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__state_q << std::endl;
  // std::cout << "cnt_q_buf " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__cnt_buf_q << std::endl;

  // n_ticks = 0;
  // while(tb->module_->out_vld_o != 1) {
  //   tb->tick(1);
  //   n_ticks++;
  //   std::cout << "out_vld_o_interm " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__out_vld_o_interm << std::endl;
  //   std::cout << "res_o_d_buf " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__res_o_d_buf << std::endl;
  //   std::cout << "ct_state_q " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__ct_state_q << std::endl;
  //   std::cout << "cnt_q_buf " << std::hex << (uint32_t) tb->module_->rootp->serdiv__DOT__cnt_buf_q << std::endl;
  // }
  // std::cout << "out_vld_o: " << (uint32_t) tb->module_->out_vld_o << std::endl;
  // std::cout << "Result: 0x" << std::hex << tb->module_->res_o << " after " << std::dec << n_ticks << " ticks." << std::endl;
  // for(int i = 0; i < 10; i++) {
  //   tb->tick(1);
  // }

}
