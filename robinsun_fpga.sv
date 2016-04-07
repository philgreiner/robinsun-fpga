
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module robinsun_fpga(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// SDRAM //////////
	DRAM_ADDR,
	DRAM_BA,
	DRAM_CAS_N,
	DRAM_CKE,
	DRAM_CLK,
	DRAM_CS_N,
	DRAM_DQ,
	DRAM_DQM,
	DRAM_RAS_N,
	DRAM_WE_N,

	//////////// ADC //////////
	ADC_CS_N,
	ADC_SADDR,
	ADC_SCLK,
	ADC_SDAT,

	//////////// 2x13 GPIO Header //////////
	GPIO_2,
	GPIO_2_IN,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	LT24,
	LT24_IN,

	//////////// GPIO_1, GPIO_1 connect to GPIO Default //////////
	GPIO1,
	GPIO1_IN 
);

//=======================================================
//  PORT declarations
//=======================================================

//////////// CLOCK //////////
input 		          		CLOCK_50;

//////////// LED //////////
output		     [7:0]		LED;

//////////// KEY //////////
input 		     [1:0]		KEY;

//////////// SW //////////
input 		     [3:0]		SW;

//////////// SDRAM //////////
output		    [12:0]		DRAM_ADDR;
output		     [1:0]		DRAM_BA;
output		          		DRAM_CAS_N;
output		          		DRAM_CKE;
output		          		DRAM_CLK;
output		          		DRAM_CS_N;
inout 		    [15:0]		DRAM_DQ;
output		     [1:0]		DRAM_DQM;
output		          		DRAM_RAS_N;
output		          		DRAM_WE_N;

//////////// ADC //////////
output		          		ADC_CS_N;
output		          		ADC_SADDR;
output		          		ADC_SCLK;
input 		          		ADC_SDAT;

//////////// 2x13 GPIO Header //////////
inout 		    [12:0]		GPIO_2;
input 		     [2:0]		GPIO_2_IN;

//////////// GPIO_1, GPIO_1 connect to GPIO Default //////////
inout 		    [33:0]		GPIO1;
input 		     [1:0]		GPIO1_IN;

//////////// GPIO_0 to LT24 //////////
inout 		    [33:0]		LT24;
input 		     [1:0]		LT24_IN;

wire		          			LT24_ADC_CS_N_bus;
wire		          			LT24_ADC_DCLK_bus;
wire		          			LT24_ADC_DIN_bus;
wire		          			LT24_ADC_BUSY_bus;
wire	          		   	LT24_ADC_DOUT_bus;
wire	          		   	LT24_ADC_PENIRQ_N_bus;
wire		    	[15:0]		LT24_D_bus;
wire		          			LT24_WR_N_bus;
wire		          			LT24_RD_N_bus;
wire		         			LT24_CS_N_bus;
wire		         			LT24_RESET_N_bus;
wire		         			LT24_RS_bus;
wire  	         			LT24_LCD_ON_bus;


wire [11:0] pic_mem_s2_address;
wire        pic_mem_s2_chipselect; 
wire        pic_mem_s2_clken;     
wire        pic_mem_s2_write;          
wire [15:0] pic_mem_s2_readdata;    
wire [15:0] pic_mem_s2_writedata;           
wire [1:0]  pic_mem_s2_byteenable;

wire [12:0] background_mem_s2_address;                   
wire        background_mem_s2_chipselect;               
wire        background_mem_s2_clken;                     
wire        background_mem_s2_write;                    
wire [15:0] background_mem_s2_readdata;                 
wire [15:0] background_mem_s2_writedata;                
wire [1:0]  background_mem_s2_byteenable; 

wire 			lt24_buffer_flag;

logic [15:0] LT24_to_SPI;

//=======================================================
//  REG/WIRE declarations
//=======================================================
logic [7:0] false_LED;
wire RST_N;
wire						wSPI_CLK;
wire						wSPI_CLK_n;

//=======================================================
//  Structural coding
//=======================================================

assign RST_N       = KEY[1];
assign LT24[33]	 = 1'b1; //default on

//=======================================================
//   PIC32 Interface
//=======================================================

//--- Declarations --------------------------------------

logic	PIC32_SDO1A, PIC32_SDI1A, PIC32_SCK1A, PIC32_CS_FPGA;
logic	PIC32_INT1, PIC32_INT2;
logic	PIC32_C1TX, PIC32_C1RX;
logic	PIC32_SCL3A, PIC32_SDA3A;
logic	PIC32_RESET;

logic [7:0] Config;
logic [7:0] Status;

//---- Assign GPIO_2 Header (connected to PIC32) --------

assign PIC32_SDO1A	= GPIO_2[0];
assign GPIO_2[1]		= PIC32_CS_FPGA ? 1'bz : PIC32_SDI1A;
assign PIC32_SCK1A	= GPIO_2[2];
assign PIC32_CS_FPGA	= GPIO_2[3];

assign GPIO_2[4]     = PIC32_INT1;
assign GPIO_2[5]     = PIC32_INT2;

assign PIC32_C1TX		= GPIO_2[6];
assign GPIO_2[7]		= Config[6] ? PIC32_C1RX : 1'bz;

assign PIC32_SCL3A	= GPIO_2[8];
assign PIC32_SDA3A	= GPIO_2[9];

assign PIC32_RESET	= GPIO_2[10];

always @ (posedge CLOCK_50)
	Status = {SW, 2'b00, KEY};

//=======================================================
//   GPIO1 Interface
//=======================================================

logic MotRA, MotLA, MotRB, MotLB, OdoRA, OdoRB, OdoLA, OdoLB, MotBA, MotBB, MotFHA, MotFHB, MotFVA, MotFVB;
logic S1T, S1S, S2T, S2S, S3T, S3S, S4T, S4S, S5T, S5S, S6T, S6S;
logic START, PARASOL;

// Assign motors and odometer signals

//ROBINSUN
assign MotRA	= GPIO1_IN[0];
assign MotLA	= GPIO1_IN[1];
assign MotRB	= GPIO1[0];
assign MotLB	= GPIO1[1];

//MINIBOT
//assign MotRA	= GPIO1_IN[0];
//assign MotRB	= GPIO1[0];
//assign MotLA	= GPIO1[1];
//assign MotLB	= GPIO1[2];

assign OdoRA	= GPIO1[2];
assign OdoRB	= GPIO1[3];
assign OdoLA	= GPIO1[4];
assign OdoLB	= GPIO1[5];
assign MotBA	= GPIO1[6];
assign MotBB	= GPIO1[7];
assign MotFHA	= GPIO1[8];
assign MotFHB	= GPIO1[9];
assign MotFVA	= GPIO1[10];
assign MotFVB	= GPIO1[11];

// Assign sonar signals
assign GPIO1[12] = S1T;
assign S1S		= GPIO1[13];
assign GPIO1[14] = S2T;
assign S2S		= GPIO1[15];
assign GPIO1[16] = S3T;
assign S3S		= GPIO1[17];
assign GPIO1[18] = S4T;
assign S4S		= GPIO1[19];
assign GPIO1[20] = S5T;
assign S5S		= GPIO1[21];
assign GPIO1[22] = S6T;
assign S6S		= GPIO1[23];

// Assign START and PARASOL signals
assign START	= ~GPIO1[24];
assign GPIO1[25] = PARASOL;

// PIC32 Interface
assign PIC32_C1RX    = GPIO1[29];
assign GPIO1[30]    = PIC32_C1TX;
assign GPIO1[31]    = PIC32_SCL3A;
assign GPIO1[32]    = PIC32_SDA3A;


//=======================================================
//  Instantiate encoder modules
//=======================================================
logic [15:0] speedR, speedL, speedOdoR, speedOdoL, speedB, speedFH, speedFV;

encoder MotR( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotRA),  .inB(MotRB),  .speed(speedR));
encoder MotL( .clk(CLOCK_50),	.reset(PIC32_RESET), .inA(MotLA),  .inB(MotLB),  .speed(speedL));
encoder OdoR( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(OdoRA),  .inB(OdoRB),  .speed(speedOdoR));
encoder OdoL( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(OdoLA),  .inB(OdoLB),  .speed(speedOdoL));
encoder MotB( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotBA),  .inB(MotBB),  .speed(speedB));
encoder MotFH(.clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotFHA), .inB(MotFHB), .speed(speedFH));
encoder MotFV(.clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotFVA), .inB(MotFVB), .speed(speedFV));


//=======================================================
//  Instantiate sonars modules and compute distances
//=======================================================
logic [19:0] R1, R2, R3, R4, R5, R6;
logic [7:0] dist1, dist2, dist3, dist4, dist5, dist6;
logic [15:0] sonar12, sonar34, sonar56;

sonars front( .clk(CLOCK_50), .reset(PIC32_RESET), .S1(S1S), .S2(S2S), .S3(S3S), .T1(S1T), .T2(S2T), .T3(S3T), .R1(R1), .R2(R2), .R3(R3));
sonars back ( .clk(CLOCK_50),	.reset(PIC32_RESET), .S1(S4S), .S2(S5S), .S3(S6S), .T1(S4T), .T2(S5T), .T3(S6T), .R1(R4), .R2(R5), .R3(R6));

assign dist1 = R1/2941;		assign dist2 = R2/2941;		assign dist3 = R3/2941;
assign dist4 = R4/2941;		assign dist5 = R5/2941;		assign dist6 = R6/2941;

assign sonar12 = {dist1, dist2}; 	assign sonar34 = {dist3, dist4}; 	assign sonar56 = {dist5, dist6};

//=======================================================
//  Instantiate SPI Interface
//=======================================================
logic [15:0] fromPIC, adcoutput;

assign LED[0] = START;
assign LED[7] = fromPIC[0];
assign LED[6] = PARASOL;

MySPI MySPI_instance(
	.theClock(CLOCK_50), 	  .theReset(PIC32_RESET),
	.MySPI_clk(PIC32_SCK1A),  .MySPI_cs(PIC32_CS_FPGA), .MySPI_sdi(PIC32_SDO1A), .MySPI_sdo(PIC32_SDI1A),
	.Config(Config),			  .Status(Status),
	.speedR(speedR), 			  
	.speedL(speedL),			  
	.speedOdoR(speedOdoR),   
	.speedOdoL(speedOdoL), 	  
	.speedB(speedB),			  
	.speedFH(speedFH), 		  
	.speedFV(speedFV),		  
	.sonar12(sonar12),		  .sonar34(sonar34), 	    .sonar56(sonar56),
	.lt24({START, LT24_to_SPI[14:0]}),
	.adc(adcoutput),
	.PICtoFPGA(fromPIC));

//=======================================================
//  Instantiate ADC Interface
//=======================================================

SPIPLL		U0	(
						.inclk0(CLOCK_50),
						.c0(wSPI_CLK),
						.c1(wSPI_CLK_n)
					);

ADC_CTRL		U1	(
						.iRST(KEY[0]),
						.iCLK(wSPI_CLK),
						.iCLK_n(wSPI_CLK_n),
						.iGO(KEY[1]),
						.iCH(SW[2:0]),
						.out(adcoutput),
						
						.oDIN(ADC_SADDR),
						.oCS_n(ADC_CS_N),
						.oSCLK(ADC_SCLK),
						.iDOUT(ADC_SDAT)
					);
//=======================================================
//  Timer for Parasol
//=======================================================

logic [23:0] parasoltimer;

always_ff @(posedge clk, posedge PIC32_RESET)
begin
	if(PIC32_RESET)
		parasoltimer <= 24'b0;
	else
	if(fromPIC[0])
		parasoltimer <= parasoltimer + 1'b1;
end

always_ff @(posedge clk)
if(parasoltimer > 24'd1 && fromPIC[0])
	PARASOL <= 1'b1;
else
	PARASOL <= 1'b0;
	

//=======================================================
//  Instantiate DE0_LT24 and LT24
//=======================================================
	
DE0_LT24_SOPC DE0_LT24_SOPC_inst(
		.clk_clk(CLOCK_50),          							//        clk.clk
		.reset_reset_n(RST_N),    								//      reset.reset_n
		
		// SDRAM
		.sdram_wire_addr(DRAM_ADDR),  						// sdram_wire.addr
		.sdram_wire_ba(DRAM_BA),    							//           .ba
		.sdram_wire_cas_n(DRAM_CAS_N), 						//           .cas_n
		.sdram_wire_cke(DRAM_CKE),  							//           .cke
		.sdram_wire_cs_n(DRAM_CS_N),  						//           .cs_n
		.sdram_wire_dq(DRAM_DQ),    							//           .dq
		.sdram_wire_dqm(DRAM_DQM),   							//           .dqm
		.sdram_wire_ras_n(DRAM_RAS_N),						//           .ras_n
		.sdram_wire_we_n(DRAM_WE_N),  						//           .we_n
		
		// KEY
		.from_key_export(KEY[0]),  							//   from_key.export
		
		// LEDS
		.to_led_export(false_LED),                 				//   to_led.export
		
		// OUTPUT SIGNAL
		.signal_out_external_connection_export(LT24_to_SPI),        //       signal_out_external_connection.export
		
		// LT24 - LCD
		.lt24_conduit_cs(LT24_CS_N_bus),      					//  lt24_conduit.cs
		.lt24_conduit_rs(LT24_RS_bus),     						//              .rs
		.lt24_conduit_rd(LT24_RD_N_bus),      					//              .rd
		.lt24_conduit_wr(LT24_WR_N_bus),      					//              .wr
		.lt24_conduit_data(LT24_D_bus),    	   				//              .data
		.lt24_lcd_rstn_export(LT24_RESET_N_bus),				//       lt24_lcd_rstn.export
		
		// LT24 - TOUCH
		.lt24_touch_spi_MISO(LT24_ADC_DOUT_bus),        		//      lt24_touch_spi.MISO
		.lt24_touch_spi_MOSI(LT24_ADC_DIN_bus),        		//                    .MOSI
		.lt24_touch_spi_SCLK(LT24_ADC_DCLK_bus),        		//                    .SCLK
		.lt24_touch_spi_SS_n(LT24_ADC_CS_N_bus),       		//                    .SS_n
		.lt24_touch_penirq_n_export(LT24_ADC_PENIRQ_N_bus), // lt24_touch_penirq_n.export
		.lt24_touch_busy_export(LT24_ADC_BUSY_bus),         //           lt24_touch_busy.export
		
		// PLL
		.alt_pll_c4_conduit_export(),        				//        alt_pll_c4_conduit.export
		.alt_pll_c3_conduit_export(),        				//        alt_pll_c3_conduit.export
		.alt_pll_areset_conduit_export(),    				//    alt_pll_areset_conduit.export
		.alt_pll_locked_conduit_export(),    				//    alt_pll_locked_conduit.export
		.alt_pll_phasedone_conduit_export(),  				// alt_pll_phasedone_conduit.export
		.alt_pll_c1_clk(DRAM_CLK),                      //                alt_pll_c1.clk   
		
		.pic_mem_s2_address(pic_mem_s2_address),               //                pic_mem_s2.address
		.pic_mem_s2_chipselect(pic_mem_s2_chipselect),            //                          .chipselect
		.pic_mem_s2_clken(pic_mem_s2_clken),                 //                          .clken
		.pic_mem_s2_write(pic_mem_s2_write),                 //                          .write
		.pic_mem_s2_readdata(pic_mem_s2_readdata),              //                          .readdata
		.pic_mem_s2_writedata(pic_mem_s2_writedata),             //                          .writedata
		.pic_mem_s2_byteenable(pic_mem_s2_byteenable),            //                          .byteenable
		
		.lt24_buffer_flag_external_connection_export(lt24_buffer_flag),
		
		.background_mem_s2_address(background_mem_s2_address),                   
		.background_mem_s2_chipselect(background_mem_s2_chipselect),                
		.background_mem_s2_clken(background_mem_s2_clken),                     
		.background_mem_s2_write(background_mem_s2_write),                     
		.background_mem_s2_readdata(background_mem_s2_readdata),                  
		.background_mem_s2_writedata(background_mem_s2_writedata),                 
		.background_mem_s2_byteenable(background_mem_s2_byteenable) 
	);
	
	
	LT24_buffer lt24_buf(
		.clk(CLOCK_50),          							
		.rst_n(RST_N),
		.LT24_ADC_BUSY_bus(LT24_ADC_BUSY_bus),
		.LT24_ADC_CS_N_bus(LT24_ADC_CS_N_bus),
		.LT24_ADC_DCLK_bus(LT24_ADC_DCLK_bus),
		.LT24_ADC_DIN_bus(LT24_ADC_DIN_bus),
		.LT24_ADC_DOUT_bus(LT24_ADC_DOUT_bus),
		.LT24_ADC_PENIRQ_N_bus(LT24_ADC_PENIRQ_N_bus),
		.LT24_D_bus(LT24_D_bus),
		.LT24_WR_N_bus(LT24_WR_N_bus),
		.LT24_RD_N_bus(LT24_RD_N_bus),
		.LT24_CS_N_bus(LT24_CS_N_bus),
		.LT24_RESET_N_bus(LT24_RESET_N_bus),
		.LT24_RS_bus(LT24_RS_bus),
		
		.LT24_ADC_BUSY_screen(LT24_IN[1]),
		.LT24_ADC_CS_N_screen(LT24[32]),
		.LT24_ADC_DCLK_screen(LT24[2]),
		.LT24_ADC_DIN_screen(LT24[1]),
		.LT24_ADC_DOUT_screen(LT24[0]),
		.LT24_ADC_PENIRQ_N_screen(LT24_IN[0]),
		//.LT24_D_screen({LT24[6],LT24[5],LT24[4],LT24[3],LT24[11:22]}),
		.LT24_D_screen({LT24[22],LT24[21],LT24[20],LT24[19],LT24[18],LT24[17],LT24[16],LT24[15],LT24[14],LT24[13],LT24[12],LT24[11],LT24[3],LT24[4],LT24[5],LT24[6]}),
		.LT24_WR_N_screen(LT24[9]),
		.LT24_RD_N_screen(LT24[8]),
		.LT24_CS_N_screen(LT24[23]),
		.LT24_RESET_N_screen(LT24[31]),
		.LT24_RS_screen(LT24[10]),
		
		.pic_mem_s2_address(pic_mem_s2_address),               //                pic_mem_s2.address
		.pic_mem_s2_chipselect(pic_mem_s2_chipselect),            //                          .chipselect
		.pic_mem_s2_clken(pic_mem_s2_clken),                 //                          .clken
		.pic_mem_s2_write(pic_mem_s2_write),                 //                          .write
		.pic_mem_s2_readdata(pic_mem_s2_readdata),              //                          .readdata
		.pic_mem_s2_writedata(pic_mem_s2_writedata),             //                          .writedata
		.pic_mem_s2_byteenable(pic_mem_s2_byteenable),            //                          .byteenable
		
		.lt24_buffer_flag(lt24_buffer_flag),
		
		.background_mem_s2_address(background_mem_s2_address),                   
		.background_mem_s2_chipselect(background_mem_s2_chipselect),                
		.background_mem_s2_clken(background_mem_s2_clken),                     
		.background_mem_s2_write(background_mem_s2_write),                     
		.background_mem_s2_readdata(background_mem_s2_readdata),                  
		.background_mem_s2_writedata(background_mem_s2_writedata),                 
		.background_mem_s2_byteenable(background_mem_s2_byteenable) 
	);

endmodule
