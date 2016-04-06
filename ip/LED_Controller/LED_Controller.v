// Revision History :
// --------------------------------------------------------------------
//   Ver  :| Author            :| Mod. Date :| Changes Made:
// --------------------------------------------------------------------

`timescale 1 ps / 1 ps
module LED_Controller (
		input  wire       CLK,      //   clock_sink.clk
		input  wire       RST,      //   reset_sink.reset
		input  wire       WRITE,    // avalon_slave.write
		input  wire		  READ,		//			   .read
		input  wire [7:0] ADDR,     //             .address
		input  wire[31:0] WDATA,    //             .writedata
		output wire[31:0] RDATA,	//			   .readdata
		output wire [7:0] LED  		//  conduit_end.export
	);

	assign LED = {1'b1,LED_reg};
	
	reg [6:0] LED_reg;
	
	always @(posedge CLK)
		if(RST)
			LED_reg <= 7'h1;
		else if(WRITE)
			LED_reg <= WDATA[6:0];

endmodule
	
