import "DPI-C" function string getenv(input string env_name);

module tb_top();

    timeunit 1ns;
    timeprecision 10ps;

    localparam time CLK_PERIOD          = 50ns;
    localparam time APPL_DELAY          = 10ns;
    localparam time ACQ_DELAY           = 30ns;
    localparam unsigned RST_CLK_CYCLES  = 10;
    localparam unsigned TOT_STIMS       = 10000;
    localparam unsigned TIMEOUT_LIM     = 1000;

    localparam type addr_t = logic [63:0];
    localparam type data_t = logic [63:0];
    localparam type strb_t = logic [7:0];

    localparam [63:0] ADDR_STOP_SIG = 0;
    localparam [63:0] ADDR_TRAP_SIG = 8;

    logic clk;
    logic rst_n;

    logic  mem_req;
    addr_t mem_addr;
    data_t mem_wdata;
    strb_t mem_strb;
    logic  mem_we;
    data_t mem_rdata;

    clk_rst_gen #(
        .CLK_PERIOD     (CLK_PERIOD),
        .RST_CLK_CYCLES (RST_CLK_CYCLES)
    ) i_clk_rst_gen (
        .clk_o  (clk),
        .rst_no (rst_n)
    );

    ariane_tiny_soc i_dut (
        .clk_i(clk),
        .rst_ni(rst_n),
        .mem_req_o(mem_req),
        .mem_addr_o(mem_addr),
        .mem_wdata_o(mem_wdata),
        .mem_strb_o(mem_strb),
        .mem_we_o(mem_we),
        .mem_rdata_o(mem_rdata)
    );

    initial begin: application_block
        wait (rst_n);

        @(posedge clk);
        #(APPL_DELAY);
    end

    initial begin: acquisition_block
        bit got_stop_req;
        int remaining_before_stop;
        int step_id;
        int simlen;

        int int_req_dump_id = 1;
        int float_req_dump_id = 0;

        wait (rst_n);

        got_stop_req = 0;
        step_id = 0;
        remaining_before_stop = 50;
        simlen = getenv("SIMLEN").atoi();

        forever begin
            @(posedge clk);
            #(ACQ_DELAY);

            // Check whether got a stop request
            if (!got_stop_req &&
                    mem_req &&
                    mem_we &&
                    // mem_wdata == 0 &&
                    mem_addr == ADDR_STOP_SIG) begin
                $display("Found a stop request. Stopping the benchmark after ", remaining_before_stop, " more ticks.");
                got_stop_req = 1;
            end

            // Register dumps
            if (!got_stop_req &&
                    mem_req &&
                    mem_we &&
                    // mem_wdata == 0 &&
                    mem_addr == 32'h10) begin
                if ($isunknown(mem_wdata))
                    $display("Dump of reg x%02d: 0x%16h", int_req_dump_id, 64'hbadcab1ebadcab1e);
                else
                    $display("Dump of reg x%02d: 0x%16h", int_req_dump_id, mem_wdata);
                int_req_dump_id += 1;
            end
            if (!got_stop_req &&
                    mem_req &&
                    mem_we &&
                    // mem_wdata == 0 &&
                    mem_addr == 32'h18) begin
                if ($isunknown(mem_wdata))
                    $display("Dump of reg f%02d: 0x%16h", float_req_dump_id, 64'hbadcab1ebadcab1e);
                else
                    $display("Dump of reg f%02d: 0x%16h", float_req_dump_id, mem_wdata);
                float_req_dump_id += 1;
            end

            // // Check whether got an exception signal
            // if (!got_stop_req &&
            //         mem_req &&
            //         mem_we &&
            //         // mem_wdata == 0 &&
            //         mem_addr == ADDR_TRAP_SIG) begin
            //     if (getenv("DONTSTOP_TB_ON_TRAP") == null || getenv("DONTSTOP_TB_ON_TRAP").atoi() != 1) begin
            //         $display("Found an exception signal. Stopping the benchmark after %d more ticks, since DONTSTOP_TB_ON_TRAP is not 1.", remaining_before_stop);
            //         got_stop_req = 1;
            //     end
            //     else
            //         $display("Found an exception signal. Not stopping.");
            // end

            // Decrement if got a stop request.
            if (got_stop_req)
                if (remaining_before_stop-- == 0)
                    break;

            // "Natural" stop since SIMLEN has been reached
            if (step_id == simlen-1) begin
                $display("Reached SIMLEN (%d cycles). Stopping.", simlen);
                break;
            end

            step_id++;
        end

        $stop();
    end

endmodule
