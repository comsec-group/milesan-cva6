module test#()(
    input logic [7:0] axi_rd_data,
    input logic rst_ni,
    input logic clk_i,
    output logic [1:0][7:0] dcache_rd_shift_d_o,
    output logic [1:0][7:0] dcache_rd_shift_q_o,
    input logic axi_rd_last
);

logic [1:0][7:0] dcache_rd_shift_d;
logic [1:0][7:0] dcache_rd_shift_q;
logic dcache_first_q, dcache_first_d;

assign dcache_rd_shift_d_o = dcache_rd_shift_d;
assign dcache_rd_shift_q_o = dcache_rd_shift_q;
always_comb begin
    dcache_first_d    = axi_rd_last;
    dcache_rd_shift_d = dcache_rd_shift_q;
    dcache_rd_shift_d = { axi_rd_data , dcache_rd_shift_q[1] };
    if(dcache_rd_first_q)
        dcache_rd_shift_d[0] = axi_rd_data;
end
always @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
        dcache_rd_shift_q <= '0;
        dcache_first_q <= 1'b1;
    end else begin
        dcache_rd_shift_q <= dcache_rd_shift_d;
        dcache_first_q <= dcache_first_d;
    end
end
endmodule