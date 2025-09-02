#include "Vtest__Syms.h"
#include "Vtest___024root.h"
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
  tb->tick();
  tb->module_->rst_ni = 1;
  tb->tick();
  tb->module_->rst_ni = 0;
  tb->tick();
  tb->module_->rst_ni = 1;
  tb->module_->axi_rd_last = 1;
  tb->tick();


  tb->module_->axi_rd_last = 0;
  tb->module_->axi_rd_data = 0x12;
  tb->module_->axi_rd_data_t0 = 0x12;
  
  tb->tick();


  std::cout << "axi_rd_data is " << std::hex <<  (std::uint32_t) tb->module_->axi_rd_data << std::endl;
  std::cout << "axi_rd_data_t0 is " << std::hex <<  (std::uint32_t)tb->module_->axi_rd_data_t0  << std::endl;
  std::cout << "dcache_rd_shift_d_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o  << std::endl;
  std::cout << "dcache_rd_shift_d_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o_t0  << std::endl;
  std::cout << "dcache_rd_shift_q_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o  << std::endl;
  std::cout << "dcache_rd_shift_q_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o_t0  << std::endl;
  std::cout << "rst_ni is " << std::hex <<  (std::uint32_t)tb->module_->rst_ni  << std::endl;
  std::cout << std::endl;

  tb->module_->axi_rd_data = 0x34;
  tb->module_->axi_rd_data_t0 = 0x34;

  tb->tick();



  std::cout << "axi_rd_data is " << std::hex <<  (std::uint32_t)tb->module_->axi_rd_data << std::endl;
  std::cout << "axi_rd_data_t0 is " << std::hex <<  (std::uint32_t)tb->module_->axi_rd_data_t0  << std::endl;
  std::cout << "dcache_rd_shift_d_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o  << std::endl;
  std::cout << "dcache_rd_shift_d_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o_t0  << std::endl;
  std::cout << "dcache_rd_shift_q_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o  << std::endl;
  std::cout << "dcache_rd_shift_q_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o_t0  << std::endl;
  std::cout << "rst_ni is " << std::hex <<  (std::uint32_t)tb->module_->rst_ni  << std::endl;
  std::cout << std::endl;

  tb->module_->axi_rd_data = 0x56;
  tb->module_->axi_rd_data_t0 = 0x56;

  tb->tick();


  std::cout << "axi_rd_data is " << std::hex <<  (std::uint32_t) tb->module_->axi_rd_data << std::endl;
  std::cout << "axi_rd_data_t0 is " << std::hex <<  (std::uint32_t)tb->module_->axi_rd_data_t0  << std::endl;
  std::cout << "dcache_rd_shift_d_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o  << std::endl;
  std::cout << "dcache_rd_shift_d_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o_t0  << std::endl;
  std::cout << "dcache_rd_shift_q_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o  << std::endl;
  std::cout << "dcache_rd_shift_q_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o_t0  << std::endl;
  std::cout << "rst_ni is " << std::hex <<  (std::uint32_t)tb->module_->rst_ni  << std::endl;
  std::cout << std::endl;


  tb->module_->axi_rd_data = 0x78;
  tb->module_->axi_rd_data_t0 = 0x78;

  tb->tick();


  std::cout << "axi_rd_data is " << std::hex <<  (std::uint32_t)tb->module_->axi_rd_data << std::endl;
  std::cout << "axi_rd_data_t0 is " << std::hex <<  (std::uint32_t)tb->module_->axi_rd_data_t0  << std::endl;
  std::cout << "dcache_rd_shift_d_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o  << std::endl;
  std::cout << "dcache_rd_shift_d_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_d_o_t0  << std::endl;
  std::cout << "dcache_rd_shift_q_o is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o  << std::endl;
  std::cout << "dcache_rd_shift_q_o_t0 is " << std::hex <<  (std::uint32_t)tb->module_->dcache_rd_shift_q_o_t0  << std::endl;
  std::cout << "rst_ni is " << std::hex <<  (std::uint32_t)tb->module_->rst_ni  << std::endl;
  std::cout << std::endl;



  ////////
  // Run the testbench.
  ////////
  
}
