
module DE0_LT24_SOPC (
	alt_pll_areset_conduit_export,
	alt_pll_c1_clk,
	alt_pll_c3_clk,
	alt_pll_locked_conduit_export,
	alt_pll_phasedone_conduit_export,
	background_mem_s2_address,
	background_mem_s2_chipselect,
	background_mem_s2_clken,
	background_mem_s2_write,
	background_mem_s2_readdata,
	background_mem_s2_writedata,
	background_mem_s2_byteenable,
	clk_clk,
	from_key_export,
	lt24_buffer_flag_external_connection_export,
	lt24_conduit_cs,
	lt24_conduit_rs,
	lt24_conduit_rd,
	lt24_conduit_wr,
	lt24_conduit_data,
	lt24_lcd_rstn_export,
	lt24_touch_busy_export,
	lt24_touch_penirq_n_export,
	lt24_touch_spi_MISO,
	lt24_touch_spi_MOSI,
	lt24_touch_spi_SCLK,
	lt24_touch_spi_SS_n,
	pic_mem_s2_address,
	pic_mem_s2_chipselect,
	pic_mem_s2_clken,
	pic_mem_s2_write,
	pic_mem_s2_readdata,
	pic_mem_s2_writedata,
	pic_mem_s2_byteenable,
	reset_reset_n,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	signal_out_external_connection_export,
	to_led_export,
	epcs_flash_controller_0_external_dclk,
	epcs_flash_controller_0_external_sce,
	epcs_flash_controller_0_external_sdo,
	epcs_flash_controller_0_external_data0);	

	input		alt_pll_areset_conduit_export;
	output		alt_pll_c1_clk;
	output		alt_pll_c3_clk;
	output		alt_pll_locked_conduit_export;
	output		alt_pll_phasedone_conduit_export;
	input	[12:0]	background_mem_s2_address;
	input		background_mem_s2_chipselect;
	input		background_mem_s2_clken;
	input		background_mem_s2_write;
	output	[15:0]	background_mem_s2_readdata;
	input	[15:0]	background_mem_s2_writedata;
	input	[1:0]	background_mem_s2_byteenable;
	input		clk_clk;
	input		from_key_export;
	output		lt24_buffer_flag_external_connection_export;
	output		lt24_conduit_cs;
	output		lt24_conduit_rs;
	output		lt24_conduit_rd;
	output		lt24_conduit_wr;
	output	[15:0]	lt24_conduit_data;
	output		lt24_lcd_rstn_export;
	input		lt24_touch_busy_export;
	input		lt24_touch_penirq_n_export;
	input		lt24_touch_spi_MISO;
	output		lt24_touch_spi_MOSI;
	output		lt24_touch_spi_SCLK;
	output		lt24_touch_spi_SS_n;
	input	[11:0]	pic_mem_s2_address;
	input		pic_mem_s2_chipselect;
	input		pic_mem_s2_clken;
	input		pic_mem_s2_write;
	output	[15:0]	pic_mem_s2_readdata;
	input	[15:0]	pic_mem_s2_writedata;
	input	[1:0]	pic_mem_s2_byteenable;
	input		reset_reset_n;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[15:0]	sdram_wire_dq;
	output	[1:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	output	[15:0]	signal_out_external_connection_export;
	output	[7:0]	to_led_export;
	output		epcs_flash_controller_0_external_dclk;
	output		epcs_flash_controller_0_external_sce;
	output		epcs_flash_controller_0_external_sdo;
	input		epcs_flash_controller_0_external_data0;
endmodule
