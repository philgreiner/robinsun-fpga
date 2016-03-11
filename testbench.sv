`timescale 1ns/1ps

module testbench();
	logic 	clk;
	logic 	reset;
	logic	S1S, S2S, S3S, S1T, S2T, S3T;
	logic [19:0] R1, R2, R3; 
	
	
	// Instantiate the device under test
	sonars front( .clk(clk), .reset(reset),
		.S1(S1S), .S2(S2S), .S3(S3S), 
		.T1(S1T), .T2(S2T), .T3(S3T), 
		.R1(R1), .R2(R2), .R3(R3));

	initial
	begin
		clk		= 1'b0;
		reset	= 1'b1;
		S1S 	= 1'b0;
		S2S 	= 1'b0;
		S3S 	= 1'b0;
		#15;
		reset	= 1'b0;
	end

	always begin
		clk = ~clk;
		#5;
	end
	
endmodule

