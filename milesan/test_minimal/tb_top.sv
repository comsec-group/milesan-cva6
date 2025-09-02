
module tb_top;

    timeunit 1ns;
    timeprecision 10ps;
    logic [3:0] axi_rd_data;
    logic [3:0] axi_rd_data_t0;

    test i_test (
        .axi_rd_data(axi_rd_data),
        .axi_rd_data_t0[axi_rd_data_t0]
    );


    initial begin: acquisition_block
        axi_rd_data <= 0;
        axi_rd_data_t0 <= 0;

    end

endmodule
