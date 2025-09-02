#include "Vparamodsimplif_f62e329a4238ff51358f__Syms.h"
#include "Vparamodsimplif_f62e329a4238ff51358f___024root.h"
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
  
}
