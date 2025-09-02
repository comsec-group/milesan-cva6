
include ../Makefile

build: ../generated/out/vanilla.sv run_test_trace.core
	rm -f fusesoc.conf
	fusesoc library add run_test_trace .
	fusesoc run --build run_test_trace

generated: 
	mkdir -p $@

generated/serdiv_ift.sv: $(MILESAN_YS)/drfuzz.ys.tcl ../generated/sv2v_out.v | generated
	DECOMPOSE_MEMORY=1 VERILOG_INPUT=$(word 2,$^) CELLIFT_EN=1 VERILOG_OUTPUT=$@ TOP_MODULE=serdiv EXCLUDE_SIGNALS=$(EXCLUDE_SIGNALS) VERBOSE=$(VERBOSE) $(YOSYS) -c $<

build_ift: generated/serdiv_ift.sv
	rm -f fusesoc.conf
	fusesoc library add run_test_ift_trace .
	fusesoc run --build run_test_ift_trace

run_ift: build_ift
	./build/run_test_ift_trace_0.1/default-verilator/Vserdiv

rerun_ift: 
	./build/run_test_ift_trace_0.1/default-verilator/Vserdiv
	
run: build
	./build/run_test_trace_0.1/default-verilator/Vserdiv


rerun: 
	./build/run_test_trace_0.1/default-verilator/Vserdiv


recompile: build/run_test_trace_0.1
	rm -f $</default-verilator/toplevel.o
	rm -f $</default-verilator/Vserdiv
	cp -r toplevel.cc $</default-verilator/src/run_test_trace_0.1/
	cp -r testbench.h $</default-verilator/src/run_test_trace_0.1/
	make -C $</default-verilator -j $(MILESAN_JOBS)

.PHONY: clean recompile

clean_module:
	rm -r generated
	rm -r build


