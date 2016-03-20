module robinsun_fpga(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// 2x13 GPIO Header //////////
	GPIO_2,
	GPIO_2_IN,

	//////////// GPIO_0, GPIO_0 connect to LTM - 4.3" LCD and Touch //////////
	LCDLTM_ADC_BUSY,
	LCDLTM_ADC_DCLK,
	LCDLTM_ADC_DIN,
	LCDLTM_ADC_DOUT,
	LCDLTM_ADC_PENIRQ_n,
	LCDLTM_B,
	LCDLTM_DEN,
	LCDLTM_G,
	LCDLTM_GREST,
	LCDLTM_HD,
	LCDLTM_NCLK,
	LCDLTM_R,
	LCDLTM_SCEN,
	LCDLTM_SDA,
	LCDLTM_VD,

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

//////////// 2x13 GPIO Header //////////
inout 		    [12:0]		GPIO_2;
input 		     [2:0]		GPIO_2_IN;

//////////// GPIO_0, GPIO_0 connect to LTM - 4.3" LCD and Touch //////////
input 		          		LCDLTM_ADC_BUSY;
output		          		LCDLTM_ADC_DCLK;
output		          		LCDLTM_ADC_DIN;
input 		          		LCDLTM_ADC_DOUT;
input 		          		LCDLTM_ADC_PENIRQ_n;
output		     [7:0]		LCDLTM_B;
output		          		LCDLTM_DEN;
output		     [7:0]		LCDLTM_G;
output		          		LCDLTM_GREST;
output		          		LCDLTM_HD;
output		          		LCDLTM_NCLK;
output		     [7:0]		LCDLTM_R;
output		          		LCDLTM_SCEN;
inout 		          		LCDLTM_SDA;
output		          		LCDLTM_VD;

//////////// GPIO_1, GPIO_1 connect to GPIO Default //////////
inout 		    [33:0]		GPIO1;
input 		     [1:0]		GPIO1_IN;


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
assign MotRA	= GPIO1_IN[0];
assign MotLA	= GPIO1_IN[1];
assign MotRB	= GPIO1[0];
assign MotLB	= GPIO1[1];
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
assign START	= GPIO1[24];
assign PARASOL	= GPIO1[25];

// PIC32 Interface
assign PIC32_C1RX    = GPIO1[29];
assign GPIO1[30]    = PIC32_C1TX;
assign GPIO1[31]    = PIC32_SCL3A;
assign GPIO1[32]    = PIC32_SDA3A;


//=======================================================
//  Instantiate encoder modules
//=======================================================
logic [7:0] dirR, dirL, dirOdoR, dirOdoL, dirB, dirFH, dirFV;
logic [15:0] speedR, speedL, speedOdoR, speedOdoL, speedB, speedFH, speedFV;

encoder MotR( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotRA),  .inB(MotRB), .direction(dirR),  .speed(speedR));
encoder MotL( .clk(CLOCK_50),	.reset(PIC32_RESET), .inA(MotLA),  .inB(MotLB), .direction(dirL),  .speed(speedL));
encoder OdoR( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(OdoRA),  .inB(OdoRB),  .direction(dirOdoR), .speed(speedOdoR));
encoder OdoL( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(OdoLA),  .inB(OdoLB),  .direction(dirOdoL), .speed(speedOdoL));
encoder MotB( .clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotBA),  .inB(MotBB),  .direction(dirB), 	 .speed(speedB));
encoder MotFH(.clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotFHA), .inB(MotFHB), .direction(dirFH), 	 .speed(speedFH));
encoder MotFV(.clk(CLOCK_50), .reset(PIC32_RESET), .inA(MotFVA), .inB(MotFVB), .direction(dirFV), 	 .speed(speedFV));


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

MySPI MySPI_instance(
	.theClock(CLOCK_50), 	  .theReset(PIC32_RESET),
	.MySPI_clk(PIC32_SCK1A),  .MySPI_cs(PIC32_CS_FPGA), .MySPI_sdi(PIC32_SDO1A), .MySPI_sdo(PIC32_SDI1A),
	.Config(Config),			  .Status(Status),
	.speedR(speedR), 			  .dirR(dirR),
	.speedL(speedL),			  .dirL(dirL), 
	.speedOdoR(speedOdoR),    .dirOdoR(dirOdoR), 
	.speedOdoL(speedOdoL), 	  .dirOdoL(dirOdoL),
	.speedB(speedB),			  .dirB(dirB),
	.speedFH(speedFH), 		  .dirFH(dirFH),
	.speedFV(speedFV),		  .dirFV(dirFV),
	.sonar12(sonar12),		  .sonar34(sonar34), 	    .sonar56(sonar56));

endmodule
