test.v: test.sv
	 sv2v -E=UnbasedUnsized -DSYNTHESIS -DVERILATOR test.sv -w test.v

test_cellifted.v: test.v
	yosys test.tcl

build_test_notrace: test_cellifted.v run_test_notrace.core
	rm -f fusesoc.conf
	fusesoc library add run_test_notrace .
	fusesoc run --build run_test_notrace


run: build_test_notrace
	./build/run_test_notrace_0.1/default-verilator/Vtest


rerun: 
	./build/run_test_notrace_0.1/default-verilator/Vtest

.PHONY: clean

clean:
	rm test.v test_cellifted.v
	rm -r build

