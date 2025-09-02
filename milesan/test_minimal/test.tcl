yosys read_verilog -sv test.v
yosys proc
yosys cellift
yosys opt
yosys write_verilog test_cellifted.v