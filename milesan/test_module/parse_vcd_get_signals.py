#%%
from vcdvcd import VCDVCD
import re
#%%
PATH_TO_VCD = "/mnt/milesan-data/cva6/925844_cva6_0_118/rtl925844_cva6_0_118.trace.vcd"
vcd = VCDVCD(PATH_TO_VCD, store_scopes=True,only_sigs=False)
#%%
TOP_MODULE ="TOP.ariane_tiny_soc.i_ariane_mem_top.i_ariane.i_cva6.genblk3.i_cache_subsystem.i_adapter"
IO_SIGS = [i.strip() for i in "clk_i, rst_ni, icache_data_req_i, icache_data_ack_o, icache_data_i, icache_rtrn_vld_o, icache_rtrn_o, dcache_data_req_i, dcache_data_ack_o, dcache_data_i, dcache_rtrn_vld_o, dcache_rtrn_o, axi_req_o, axi_resp_i, axi_req_o_t0, axi_resp_i_t0, icache_rtrn_o_t0, icache_data_req_i_t0, icache_data_i_t0, icache_data_ack_o_t0, dcache_rtrn_vld_o_t0, dcache_rtrn_o_t0, dcache_data_req_i_t0, icache_rtrn_vld_o_t0, dcache_data_i_t0, dcache_data_ack_o_t0".split(",")]
TOPLEVEL_TEMPLATE = "/mnt/milesan-designs/milesan-cva6/milesan/test_module/toplevel_template.cc"
TOPLEVEL = "/mnt/milesan-designs/milesan-cva6/milesan/test_module/toplevel.cc"
# ENDTIME=82402
ENDTIME=10000
N_RST_TICKS = 10

#%%
io_sigs = []
input_signals = []
for signal in vcd.scopes[TOP_MODULE]:
    if "[" in signal:
        signal_clip = signal.split("[")[0]
    else:
        signal_clip = signal
    if signal_clip.endswith("_i"):
        print(f"input {signal}")
        input_signals += [signal]
    elif signal_clip.endswith("_o"):
        print(f"output {signal}")
    elif signal_clip.endswith("_i_t0"):
        print(f"input taint {signal}")
        input_signals += [signal]
    elif signal_clip.endswith("_o_t0"):
        print(f"output taint {signal}")
    elif signal_clip == "clk_i":
        print(f"CLK {signal}")
    elif signal_clip == "rst_ni":
        print(f"RST {signal}")
    else:
        continue
    io_sigs += [signal]

assert len(io_sigs) == len(IO_SIGS)

#%%
time_value_pairs = {i:{} for i in range(0,ENDTIME)}
for i in range(0,ENDTIME):
    for signal in input_signals:
        vc_ts = [t[0] for t in vcd[f"{TOP_MODULE}.{signal}"].tv]
        if i in vc_ts:
            value =  vcd[f"{TOP_MODULE}.{signal}"].tv[vc_ts.index(i)][-1]
            if "[" in signal:
                signal_clip = signal.split("[")[0]
            else:
                signal_clip = signal
            time_value_pairs[i][signal_clip] = value
#%%
application_block = ""
application_block += "tb->module_->rst_ni = 0;\n"
for _ in range(N_RST_TICKS):
    application_block += "tb->tick();\n"
application_block += "tb->module_->rst_ni = 1;\n"
clk_i = 1
for time,signal_values in time_value_pairs.items():
    for name, value_bin in signal_values.items():
        value = int(value_bin,2)
        if "clk_i" in name:
            if not value and clk_i: # posedge
                application_block += "tb->tick();\n"
                clk_i = value
            else:
                clk_i = 1
        else:
            n_bits = len(value_bin)
            if n_bits <= 32 or "icache_data" in name:
                application_block += f"tb->module_->{name} = {hex(value)};\n"
            else:
                n_words = n_bits//32+1
                assert n_words > 1
                for i in range(n_words):
                    application_block += f"tb->module_->{name}[{i}] = {hex((value>>i*32)&0xFFFFFFFF)};\n"
# %%
with open(TOPLEVEL_TEMPLATE, "r") as f:
    toplevel_template = f.read()

toplevel = ""
for line in toplevel_template.split("\n"):
    toplevel += line + "\n"
    if "Testbench *tb = new Testbench(cl_get_tracefile());" in line:
        toplevel += application_block

with open(TOPLEVEL, "w") as f:
    f.write(toplevel)
#%%
PATH_TO_VCD_TEST = "test.vcd"
vcd_test = VCDVCD(PATH_TO_VCD, store_scopes=True,only_sigs=False)
#%%
io_sigs_test = []
input_signals_test = []
for signal in vcd.scopes[TOP_MODULE]:
    if "[" in signal:
        signal_clip = signal.split("[")[0]
    else:
        signal_clip = signal
    if signal_clip.endswith("_i"):
        print(f"input {signal}")
        input_signals_test += [signal]
    elif signal_clip.endswith("_o"):
        print(f"output {signal}")
    elif signal_clip.endswith("_i_t0"):
        print(f"input taint {signal}")
        input_signals += [signal]
    elif signal_clip.endswith("_o_t0"):
        print(f"output taint {signal}")
    elif signal_clip == "clk_i":
        print(f"CLK {signal}")
    elif signal_clip == "rst_ni":
        print(f"RST {signal}")
    else:
        continue
    io_sigs_test += [signal]

assert len(io_sigs_test) == len(IO_SIGS)
assert len(io_sigs) == len(io_sigs_test)
for sig in io_sigs_test:
    assert sig in io_sigs
for sig in io_sigs:
    assert sig in io_sigs_test
#%%
#%%
time_value_pairs_test = {i:{} for i in range(0,ENDTIME)}
for i in range(0,ENDTIME):
    for signal in input_signals_test:
        vc_ts = [t[0] for t in vcd[f"{TOP_MODULE}.{signal}"].tv]
        if i in vc_ts:
            value =  vcd[f"{TOP_MODULE}.{signal}"].tv[vc_ts.index(i)][-1]
            if "[" in signal:
                signal_clip = signal.split("[")[0]
            else:
                signal_clip = signal
            time_value_pairs_test[i][signal_clip] = value
            assert signal_clip in time_value_pairs[i], f"{signal_clip} not in original vcd!"
            assert time_value_pairs[i][signal_clip] == value, f"Value mismatch between generated inputs and original vcd: {hex(value)} != {hex(time_value_pairs[i][signal_clip])}"