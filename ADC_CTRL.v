module ADC_CTRL	(	
	input iRST,
	input iCLK,
	input iCLK_n,
	input iGO,
	input [2:0] iCH,
	output [15:0] out,
	output	oDIN,
	output 	oCS_n,
	output 	oSCLK,
	input 	iDOUT);

reg					data;
reg					go_en;
reg					sclk;
reg		[3:0]		cont;
reg		[3:0]		m_cont;
reg		[11:0]	adc_data;
reg		[15:0]	outputData;
reg 					CHANNEL;

assign	oCS_n		=	~go_en;
assign	oSCLK		=	(go_en)? iCLK:1;
assign	oDIN		=	data;
assign	out		=	outputData;

always@(posedge iGO or negedge iRST)
begin
	if(!iRST)
		go_en	<=	0;
	else
	begin
		if(iGO)
			go_en	<=	1;
	end
end

always@(posedge iCLK or negedge go_en)
begin
	if(!go_en)
		cont	<=	0;
	else
	begin
		if(iCLK)
			cont	<=	cont + 1;
	end
end

always@(posedge iCLK_n)
begin
	if(iCLK_n)
		m_cont	<=	cont;
end

always@(posedge iCLK_n or negedge go_en)
begin
	if(!go_en)
		data	<=	0;
	else
	begin
		if(iCLK_n)
		begin
			if (cont == 2)
				data	<=	0;
			else if (cont == 3)
				data	<=	0;
			else if (cont == 4)
				data	<=	CHANNEL;
			else
				data	<=	0;
		end
	end
end

always@(posedge iCLK or negedge go_en)
begin
	if(!go_en)
	begin
		CHANNEL <= 0;
		adc_data	<=	0;
		outputData		<=	16'h00;
	end
	else
	begin
		if(iCLK)
		begin
			if (m_cont == 4)
				adc_data[11]	<=	iDOUT;
			else if (m_cont == 5)
				adc_data[10]	<=	iDOUT;
			else if (m_cont == 6)
				adc_data[9]		<=	iDOUT;
			else if (m_cont == 7)
				adc_data[8]		<=	iDOUT;
			else if (m_cont == 8)
				adc_data[7]		<=	iDOUT;
			else if (m_cont == 9)
				adc_data[6]		<=	iDOUT;
			else if (m_cont == 10)
				adc_data[5]		<=	iDOUT;
			else if (m_cont == 11)
				adc_data[4]		<=	iDOUT;
			else if (m_cont == 12)
				adc_data[3]		<=	iDOUT;
			else if (m_cont == 13)
				adc_data[2]		<=	iDOUT;
			else if (m_cont == 14)
				adc_data[1]		<=	iDOUT;
			else if (m_cont == 15)
				adc_data[0]		<=	iDOUT;
			else if (m_cont == 1)
			begin
				outputData	<=	CHANNEL ? {adc_data[11:4], outputData[7:0]} : {outputData[15:8], adc_data[11:4]};
				CHANNEL <= ~CHANNEL;
			end
		end
	end
end

endmodule