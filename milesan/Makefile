# Copyright Flavien Solt, ETH Zurich.
# Licensed under the General Public License, Version 3.0, see LICENSE for details.
# SPDX-License-Identifier: GPL-3.0-only

ifeq "" "$(MILESAN_ENV_SOURCED)"
$(error Please re-source env.sh first, in the meta repo, and run from there, not this repo. See README.md in the meta repo)
endif

PYTHON ?= python3
YOSYS ?= yosys
RESOURCEWRAPPER_TAG ?= cva6

TOP_MODULE = ariane_mem_top # Top module for the Yosys instrumentation.
TOP_SOC = ariane_tiny_soc
TOP_EXECUTABLE_NAME = V$(TOP_SOC)
VCD_REGEX = *
# For drfuzz
TOP_RESET = rst_ni
EXCLUDE_SIGNALS ?= clk_i,rst_ni

TARGET_NAMES = vanilla drfuzz_mem passthrough

REMOVE_MODULES = pad_functional_pd pad_functional_pu axi_lite_to_apb axi_lite_to_apb_intf axi_lite_xbar axi_lite_to_axi axi_modify_address axi_to_axi_lite axi_to_axi_lite_intf axi_to_axi_lite_id_reflect generic_rom sub_per_hash cb_filter hash_block ecc_decode ecc_encode ariane_testharness cva6_mmu_sv32 clk_mux_glitch_free clk_or_tree find_first_one

# Use common design Makefile rules
include $(MILESAN_DESIGN_PROCESSING_ROOT)/common/design.mk

# This target makes the design until the Yosys instrumentation. From there on, the Makefile can run in parallel for the various instrumentation targets.
before_instrumentation: generated/sv2v_out.v

#
# 1. Gather the source code.
#

generated/bender.sources: | generated
	bender sources -f -t cv64a6_imafdc_sv39 -t ariane_mem_top > generated/tmp.sources
	sed -i "s/synopsys_sram/util\/sram/g" generated/tmp.sources
	grep -v "_trace" generated/tmp.sources > $@
	rm -rf generated/tmp.sources

#
# 2. Pickle the source code.
#

ARIANE_SMALLER_CACHES = -DARIANE_SMALLER_CACHES

generated/ariane_pickled.sv: generated/bender.sources
	morty -f $< -DVERILATOR -DSYNTHESIS $(ARIANE_SMALLER_CACHES) -o generated/pickled_interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/ascend_package_in_pickle.py cva6_config_pkg generated/pickled_interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/remove_pragma_translate_off.py generated/pickled_interm.sv generated/pickled_interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/remove_attributes.py generated/pickled_interm.sv generated/pickled_interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/remove_modules.py generated/pickled_interm.sv $@ $(REMOVE_MODULES)
	# rm -rf generated/pickled_interm.sv
	touch $@.log

#
# 3. Apply sv2v and discard signal strengths.
#

generated/sv2v_out.v: generated/ariane_pickled.sv
#   Write-back cache
	sv2v -E=UnbasedUnsized -DSYNTHESIS -DVERILATOR $(ARIANE_SMALLER_CACHES) $< -w $@
	sed -i 's/assign (weak0, weak1)/assign/g'     $@
	sed -i 's/assign (strong0, strong1)/assign/g' $@
	sed -i 's/assign (pull0, pull1)/assign/g'     $@
# Add newline in the end of the file because sv2v does not.
	echo  >> $@

#
# 5. Apply some Python scripts to:
# - compacify the wide concatenations.
# - properly run with Verilator.
#

generated/out/vanilla.sv: generated/ariane_pickled.sv | generated/out
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/remove_modules.py $< generated/interm.sv $(REMOVE_MODULES)
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/compress_concats.py generated/interm.sv $@

generated/out/vanilla.sv.log: | generated/out
	touch $@

generated/out/passthrough.sv: $(MILESAN_YS)/passthrough.ys.tcl generated/sv2v_out.sv  | generated/out logs
	VERILOG_INPUT=$(word 2,$^) VERILOG_OUTPUT=$@ TOP_MODULE=$(TOP_MODULE) $(YOSYS) -c $< -l $@.log

generated/out/drfuzz_mem_precompact.sv: $(MILESAN_YS)/drfuzz.ys.tcl generated/sv2v_out.v | generated/out logs
	DECOMPOSE_MEMORY=1 VERILOG_INPUT=$(word 2,$^) CELLIFT_EN=1 VERILOG_OUTPUT=$@ TOP_MODULE=$(TOP_MODULE) EXCLUDE_SIGNALS=$(EXCLUDE_SIGNALS) VERBOSE=$(VERBOSE) $(YOSYS) -c $< -l $@.log

generated/out/drfuzz_mem.sv: generated/out/drfuzz_mem_precompact.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/remove_modules.py $< generated/interm.sv ariane_testharness cva6_mmu_sv32 pad_functional_pd pad_functional_pu
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/expand_left_operand.py generated/interm.sv generated/interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/expand_right_operand.py generated/interm.sv generated/interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/compress_concats.py generated/interm.sv generated/interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/divide_concat_into_subconcats.py generated/interm.sv generated/interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/correct_too_wide_lvalues.py generated/interm.sv generated/interm.sv
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/simplify_yosysout.py generated/interm.sv $@
	cp $<.log $@.log

# Core files
CORE_FILES_NOTRACE=$(patsubst %,run_%_notrace.core, $(TARGET_NAMES))
$(CORE_FILES_NOTRACE): run_%.core: run_%.core.template
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/gen_corefiles.py $< $@

CORE_FILES_TRACE=$(patsubst %,run_%_trace.core, $(TARGET_NAMES))
$(CORE_FILES_TRACE): run_%.core: run_%.core.template
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/gen_corefiles.py $< $@

CORE_FILES_TRACE_FST=$(patsubst %,run_%_trace_fst.core, $(TARGET_NAMES))
$(CORE_FILES_TRACE_FST): run_%.core: run_%.core.template
	$(PYTHON) $(MILESAN_PYTHON_COMMON)/gen_corefiles.py $< $@

#
# Recompile, if only the software has changed since the previous step.
#

RECOMPILE_TARGETS_NOTRACE=$(patsubst %,recompile_%_notrace, $(TARGET_NAMES))
RECOMPILE_TARGETS_TRACE=$(patsubst %,recompile_%_trace, $(TARGET_NAMES))
RECOMPILE_TARGETS_TRACE_FST=$(patsubst %,recompile_%_trace_fst, $(TARGET_NAMES))
RECOMPILE_TARGETS = $(RECOMPILE_TARGETS_NOTRACE) $(RECOMPILE_TARGETS_TRACE) $(RECOMPILE_TARGETS_TRACE_FST)

.PHONY: $(RECOMPILE_TARGETS)
$(RECOMPILE_TARGETS): recompile_%: build/run_%_0.1
	rm -f $</default-verilator/toplevel.o
	rm -f $</default-verilator/$(TOP_EXECUTABLE_NAME)
	rm -rf $</src/run_$*_0.1/dv
	rm -rf ./build/dv
	cp -r dv $</default-verilator/src/run_$*_0.1
	cp -r $(MILESAN_DESIGN_PROCESSING_ROOT)/common/dv ./build
	make -C $</default-verilator -j $(MILESAN_JOBS)


#
# Rerun a simulation.
#

RERUN_TARGETS_NOTRACE=$(patsubst %,rerun_%_notrace, $(TARGET_NAMES))
RERUN_TARGETS_TRACE=$(patsubst %,rerun_%_trace, $(TARGET_NAMES))
RERUN_TARGETS_TRACE_FST=$(patsubst %,rerun_%_trace_fst, $(TARGET_NAMES))
RERUN_TARGETS = $(RERUN_TARGETS_NOTRACE) $(RERUN_TARGETS_TRACE) $(RERUN_TARGETS_TRACE_FST)

.PHONY: $(RERUN_TARGETS) | traces
$(RERUN_TARGETS): rerun_%: build/run_%_0.1/
	$</default-verilator/$(TOP_EXECUTABLE_NAME)

#
# 7. Run with Verilator through FuseSoC
# - properly run with Verilator.
# - insert the right SRAMs.
#

# Phony targets

PREPARE_TARGETS_NOTRACE=$(patsubst %,prepare_%_notrace, $(TARGET_NAMES))
PREPARE_TARGETS_TRACE=$(patsubst %,prepare_%_trace, $(TARGET_NAMES))
PREPARE_TARGETS_TRACE_FST=$(patsubst %,prepare_%_trace_fst, $(TARGET_NAMES))
PREPARE_TARGETS = $(PREPARE_TARGETS_NOTRACE) $(PREPARE_TARGETS_TRACE) $(PREPARE_TARGETS_TRACE)
.PHONY: $(PREPARE_TARGETS)
$(PREPARE_TARGETS): prepare_%: build/run_%_0.1/default-verilator/$(TOP_EXECUTABLE_NAME)

# Actual targets

BUILD_TARGETS_NOTRACE=$(patsubst %,build/run_%_notrace_0.1/default-verilator/$(TOP_EXECUTABLE_NAME), $(TARGET_NAMES))
BUILD_TARGETS_TRACE=$(patsubst %,build/run_%_trace_0.1/default-verilator/$(TOP_EXECUTABLE_NAME), $(TARGET_NAMES))
BUILD_TARGETS_TRACE_FST=$(patsubst %,build/run_%_trace_fst_0.1/default-verilator/$(TOP_EXECUTABLE_NAME), $(TARGET_NAMES))
BUILD_TARGETS = $(BUILD_TARGETS_NOTRACE) $(BUILD_TARGETS_TRACE) $(BUILD_TARGETS_TRACE_FST)

$(BUILD_TARGETS_NOTRACE): build/run_%_notrace_0.1/default-verilator/$(TOP_EXECUTABLE_NAME): generated/out/%.sv generated/out/%.sv.log run_%_notrace.core
	rm -f fusesoc.conf
	fusesoc library add run_$*_notrace .
	fusesoc run --build run_$*_notrace
	cp $<.log $@.log
$(BUILD_TARGETS_TRACE): build/run_%_trace_0.1/default-verilator/$(TOP_EXECUTABLE_NAME): generated/out/%.sv generated/out/%.sv.log run_%_trace.core
	rm -f fusesoc.conf
	fusesoc library add run_$*_trace .
	fusesoc run --build run_$*_trace
	cp $<.log $@.log
$(BUILD_TARGETS_TRACE_FST): build/run_%_trace_fst_0.1/default-verilator/$(TOP_EXECUTABLE_NAME): generated/out/%.sv generated/out/%.sv.log run_%_trace_fst.core
	rm -f fusesoc.conf
	fusesoc library add run_$*_trace_fst .
	fusesoc run --build run_$*_trace_fst
	cp $<.log $@.log

#
# Run, potentially after compiling.
#

RUN_TARGETS_NOTRACE=$(patsubst %,run_%_notrace, $(TARGET_NAMES))
RUN_TARGETS_TRACE=$(patsubst %,run_%_trace, $(TARGET_NAMES))
RUN_TARGETS_TRACE_FST=$(patsubst %,run_%_trace_fst, $(TARGET_NAMES))
RUN_TARGETS = $(RUN_TARGETS_NOTRACE) $(RUN_TARGETS_TRACE) $(RUN_TARGETS_TRACE_FST)

$(RUN_TARGETS_TRACE) $(RUN_TARGETS_TRACE_FST): | traces
$(RUN_TARGETS): run_%: ./build/run_%_0.1/default-verilator/$(TOP_EXECUTABLE_NAME)
	cd build/run_$*_0.1/default-verilator && ./$(TOP_EXECUTABLE_NAME)

#
# Modelsim
#

MILESAN_DIR := ${shell dirname ${shell pwd}}/milesan
MODELSIM_PATH_TO_BUILD_TCL = $(MILESAN_DESIGN_PROCESSING_ROOT)/common/modelsim/modelsim_build.tcl

include $(MILESAN_DESIGN_PROCESSING_ROOT)/common/modelsim.mk
