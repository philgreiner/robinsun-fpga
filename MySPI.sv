//=======================================================

module MySPI (
	input  logic		  theClock, theReset,
	input  logic        MySPI_clk, MySPI_cs, MySPI_sdi,
	output logic  		  MySPI_sdo,
	output logic [8:0]  Config,
	input  logic [8:0]  Status,
	input  logic [15:0] speedR, speedL,
	input  logic 		  dirR, dirL);

//--- Registers Address ---------------------------------
parameter A_Config     			= 7'h00;
parameter A_Status     			= 7'h01;

parameter A_dirR					= 7'h10;
parameter A_speedR1				= 7'h11;
parameter A_speedR2				= 7'h12;
parameter A_dirL					= 7'h20;
parameter A_speedL1				= 7'h21;
parameter A_speedL2				= 7'h22;

//--- Separation of too long inputs ---------------------

logic [7:0] speedR1, speedR2, speedL1, speedL2;

assign speedR1 	    = speedR[15:8];
assign speedR2 		 = speedR[7:0];
assign speedL1 		 = speedL[15:8];
assign speedL2 		 = speedL[7:0];

//--- FSM States ----------------------------------------

typedef enum logic [3:0] {
	S_Wait, 
	S_Addr, S_Addr_00, S_Addr_01, S_Addr_11,
	S_Data, S_Data_00, S_Data_01, S_Data_11,
	S_End} statetype;

//--- Declarations --------------------------------------

statetype	SPI_state, SPI_nextstate;
logic			SPI_CLK0, SPI_CLK;
logic			SPI_CS0, SPI_CS;
logic [2:0] SPI_counter;
logic			SPI_counter_reset, SPI_counter_inc;	 
logic [7:0] SPI_address, SPI_data;
logic			SPI_address_shift;
logic			SPI_data_shift, SPI_data_load, SPI_data_update;

//--- SPI Output ----------------------------------------

assign MySPI_sdo = SPI_data[7];

//--- SPI Double Synchronization ------------------------

always @ (posedge theClock)
begin
	SPI_CLK0 <= MySPI_clk;	SPI_CLK  <= SPI_CLK0;
	SPI_CS0  <= MySPI_cs;	SPI_CS   <= SPI_CS0;
end


//--- SPI FSM -------------------------------------------

always_ff @ (posedge theClock)
	SPI_state <= SPI_nextstate;
	
always_comb
begin
	SPI_nextstate = SPI_state;
	case (SPI_state)
		S_Wait	 : if (SPI_CS) SPI_nextstate = S_Wait;
							else SPI_nextstate = S_Addr;
		S_Addr	 : SPI_nextstate = S_Addr_00;
		S_Addr_00 : if (SPI_CLK) SPI_nextstate = S_Addr_01;
		S_Addr_01 : SPI_nextstate = S_Addr_11;
		S_Addr_11 : if (SPI_CLK) SPI_nextstate = S_Addr_11;
							else if (SPI_counter == 3'b000) SPI_nextstate = S_Data;
								else SPI_nextstate = S_Addr_00;
		S_Data	 : SPI_nextstate = S_Data_00;
		S_Data_00 : if (SPI_CLK) SPI_nextstate = S_Data_01;
		S_Data_01 : SPI_nextstate = S_Data_11;
		S_Data_11 : if (SPI_CLK) SPI_nextstate = S_Data_11;
							else if (SPI_counter == 3'b000) SPI_nextstate = S_End;
								else SPI_nextstate = S_Data_00;
		S_End     : SPI_nextstate = S_Wait;
	endcase
	if (SPI_CS) SPI_nextstate = S_Wait;
end

assign SPI_counter_reset = ((SPI_state == S_Addr)    | (SPI_state == S_Data));
assign SPI_counter_inc   = ((SPI_state == S_Addr_01) | (SPI_state == S_Data_01));
assign SPI_address_shift = (SPI_state == S_Addr_01);
assign SPI_data_shift	 = (SPI_state == S_Data_01);
assign SPI_data_load		 = (SPI_state == S_Data);
assign SPI_data_update   = ((SPI_state == S_End) & SPI_address[7]);

//--- On the positive edge of the clock -----------------

always_ff @ (posedge theClock)
begin
	
	if (SPI_counter_reset) SPI_counter <= 3'b000;
		else if (SPI_counter_inc) SPI_counter <= SPI_counter + 1;
		
	if (SPI_address_shift) SPI_address <= { SPI_address[6:0], MySPI_sdi };
	
	if (SPI_data_shift) SPI_data <= { SPI_data[6:0], MySPI_sdi };
		else if (SPI_data_load)
			case (SPI_address[6:0])
				A_Config    		: SPI_data <= Config;
				A_Status    		: SPI_data <= Status;
				A_dirR				: SPI_data <= {7'b0, dirR};
				A_speedR1			: SPI_data <= speedR1;
				A_speedR2			: SPI_data <= speedR2;
				A_dirL				: SPI_data <= {7'b0, dirL};
				A_speedL1			: SPI_data <= speedL1;
				A_speedL2			: SPI_data <= speedL2;
			endcase
		
	if (theReset) Config <= 8'h00;
		else if ((SPI_data_update) & (SPI_address[6:0] == A_Config)) Config <= SPI_data;
end

endmodule

//=======================================================