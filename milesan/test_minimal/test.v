module test (
	axi_rd_data,
	rst_ni,
	clk_i,
	dcache_rd_shift_d_o,
	dcache_rd_shift_q_o,
	axi_rd_last
);
	input wire [7:0] axi_rd_data;
	input wire rst_ni;
	input wire clk_i;
	output wire [15:0] dcache_rd_shift_d_o;
	output wire [15:0] dcache_rd_shift_q_o;
	input wire axi_rd_last;
	reg [15:0] dcache_rd_shift_d;
	reg [15:0] dcache_rd_shift_q;
	reg dcache_first_q;
	reg dcache_first_d;
	assign dcache_rd_shift_d_o = dcache_rd_shift_d;
	assign dcache_rd_shift_q_o = dcache_rd_shift_q;
	always @(*) begin
		dcache_first_d = axi_rd_last;
		dcache_rd_shift_d = dcache_rd_shift_q;
		dcache_rd_shift_d = {axi_rd_data, dcache_rd_shift_q[8+:8]};
		if (dcache_rd_first_q)
			dcache_rd_shift_d[0+:8] = axi_rd_data;
	end
	always @(posedge clk_i or negedge rst_ni)
		if (!rst_ni) begin
			dcache_rd_shift_q <= '0;
			dcache_first_q <= 1'b1;
		end
		else begin
			dcache_rd_shift_q <= dcache_rd_shift_d;
			dcache_first_q <= dcache_first_d;
		end
endmodule