# Copyright Flavien Solt, ETH Zurich.
# Licensed under the General Public License, Version 3.0, see LICENSE for details.
# SPDX-License-Identifier: GPL-3.0-only

export TRACEFILE=$PWD/traces/sim_instrumented.vcd
# export COVERAGEFILE=$PWD/traces/coverage.dat
# export SIMSRAMELF=$PWD/test.elf
# export SIMSRAMELF=/scratch/flsolt/data/python-tmp/rtl411231_cva6_314712_12.elf
export SIMSRAMELF=$PWD/sw/ignoreme/build/app.elf
# export SIMSRAMELF=$PWD/sw/fuzzsample/app_buggy.elf
export SIMLEN=100000
