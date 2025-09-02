
# generated/out/test_module_cellifted.sv: $(MILESAN_YS)/passthrough.ys.tcl ../generated/out/drfuzz_mem.sv | ../generated/out logs
# 	-mkdir generated
# 	-mkdir generated/out
# 	VERILOG_INPUT=$(word 2,$^) VERILOG_OUTPUT=$@ TOP_MODULE=paramodsimplif_f62e329a4238ff51358f VERBOSE=$(VERBOSE) $(YOSYS) -c $< -l $@.log

# TOP_MODULE = paramodsimplif_f62e329a4238ff51358f
# generated/out/test_module_cellifted.sv: $(MILESAN_YS)/drfuzz.ys.tcl ../generated/sv2v_out.v | ../generated/out logs
# 	-mkdir generated
# 	-mkdir generated/out
# 	DECOMPOSE_MEMORY=1 VERILOG_INPUT=$(word 2,$^) CELLIFT_EN=1 VERILOG_OUTPUT=$@ TOP_MODULE=$(TOP_MODULE) EXCLUDE_SIGNALS=$(EXCLUDE_SIGNALS) VERBOSE=$(VERBOSE) $(YOSYS) -c $< -l $@.log



build_test_notrace: ../generated/out/drfuzz_mem.sv run_test_notrace.core
	rm -f fusesoc.conf
	fusesoc library add run_test_notrace .
	fusesoc run --build run_test_notrace


run: build_test_notrace
	./build/run_test_notrace_0.1/default-verilator/Vparamodsimplif_f62e329a4238ff51358f


rerun: 
	./build/run_test_notrace_0.1/default-verilator/Vparamodsimplif_f62e329a4238ff51358f


recompile: build/run_test_notrace_0.1
	rm -f $</default-verilator/toplevel.o
	rm -f $</default-verilator/Vparamodsimplif_f62e329a4238ff51358f
	cp -r toplevel.cc $</default-verilator/src/run_test_notrace_0.1/
	cp -r testbench.h $</default-verilator/src/run_test_notrace_0.1/
	make -C $</default-verilator -j $(MILESAN_JOBS)

.PHONY: clean recompile

clean_module:
	rm -r generated
	rm -r build


include ../Makefile