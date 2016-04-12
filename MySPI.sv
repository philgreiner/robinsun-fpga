//=======================================================

module MySPI (
	input  logic		  theClock, theReset,
	input  logic        MySPI_clk, MySPI_cs, MySPI_sdi,
	output logic  		  MySPI_sdo,
	output logic [7:0]  Config,
	input  logic [7:0]  Status,
	input  logic [15:0] speedR,
	input  logic [15:0] speedL, 
	input  logic [15:0] speedOdoR, 
	input  logic [15:0] speedOdoL, 
	input  logic [15:0] speedB,
	input  logic [15:0] speedFH,
	input  logic [15:0] speedFV,
	input  logic [15:0] sonar12, sonar34, sonar56,
	input  logic [15:0] lt24,
	input  logic [15:0] adc,
	output logic [15:0] PICtoFPGA);

//--- Registers Address ---------------------------------
parameter A_Config     			= 15'h00;
parameter A_Status     			= 15'h01;

parameter A_speedR				= 15'h22;
parameter A_speedL				= 15'h32;
parameter A_speedOdoR			= 15'h26;
parameter A_speedOdoL			= 15'h36;

parameter A_speedB				= 15'h50;
parameter A_speedFH				= 15'h52;
parameter A_speedFV				= 15'h54;

parameter A_sonar12				= 15'h41;
parameter A_sonar34				= 15'h42;
parameter A_sonar56				= 15'h43;

parameter A_lt24					= 15'h12;
parameter A_adc					= 15'h19;

parameter A_PICtoFPGA			= 15'h10;

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
logic [3:0] SPI_counter;
logic			SPI_counter_reset, SPI_counter_inc;	 
logic [15:0] SPI_address, SPI_data;
logic			SPI_address_shift;
logic			SPI_data_shift, SPI_data_load, SPI_data_update;

//--- SPI Output ----------------------------------------

assign MySPI_sdo = SPI_data[15];

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
							else if (SPI_counter == 4'b0000) SPI_nextstate = S_Data;
								else SPI_nextstate = S_Addr_00;
		S_Data	 : SPI_nextstate = S_Data_00;
		S_Data_00 : if (SPI_CLK) SPI_nextstate = S_Data_01;
		S_Data_01 : SPI_nextstate = S_Data_11;
		S_Data_11 : if (SPI_CLK) SPI_nextstate = S_Data_11;
							else if (SPI_counter == 4'b0000) SPI_nextstate = S_End;
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
assign SPI_data_update   = ((SPI_state == S_End) & SPI_address[15]);

//--- On the positive edge of the clock -----------------

always_ff @ (posedge theClock)
begin
	if (SPI_counter_reset) SPI_counter <= 4'b0000;
		else if (SPI_counter_inc) SPI_counter <= SPI_counter + 1;
		
	if (SPI_address_shift) SPI_address <= { SPI_address[14:0], MySPI_sdi };
	
	if (SPI_data_shift) SPI_data <= { SPI_data[14:0], MySPI_sdi };
		else if (SPI_data_load)
		begin
			case (SPI_address[14:0])
				A_Config    		: SPI_data <= {8'b0, Config};
				A_Status    		: SPI_data <= {8'b0, Status};
				A_speedR				: SPI_data <= speedR;
				A_speedL				: SPI_data <= speedL;
				A_speedOdoR			: SPI_data <= speedOdoR;
				A_speedOdoL			: SPI_data <= speedOdoL;
				A_sonar12			: SPI_data <= sonar12;
				A_sonar34			: SPI_data <= sonar34;
				A_sonar56			: SPI_data <= sonar56;
				A_speedB				: SPI_data <= speedB;
				A_speedFH			: SPI_data <= speedFH;
				A_speedFV			: SPI_data <= speedFV;
				A_lt24				: SPI_data <= lt24;
				A_adc					: SPI_data <= adc;
				A_PICtoFPGA			: SPI_data <= PICtoFPGA;
			endcase
			if(SPI_address[14:0] == A_speedB)
				PICtoFPGA[15] <= 1'b1;
			else
				PICtoFPGA[15] <= 1'b0;
		end
		
	if (theReset) Config <= 8'h00;
		else if ((SPI_data_update) & (SPI_address[14:0] == A_Config)) Config <= SPI_data[7:0];
	if (theReset) PICtoFPGA <= 16'h00;
		else if ((SPI_data_update) & (SPI_address[14:0] == A_PICtoFPGA)) PICtoFPGA[14:0] <= SPI_data[14:0];
		
end

endmodule

//=======================================================