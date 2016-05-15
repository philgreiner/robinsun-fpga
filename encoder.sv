/* 
* Uses the entries (A and B) from a motor encoder compute:
*		speed [15:0] --> Rotation speed (ticks per 25 ms)
*/
module encoder(
input  logic clk, reset, inA, inB,
output logic [15:0] speed);

logic [15:0] tickcount1, tickcount2, tickcount3, tickcount4;
logic [31:0] clkcount;
logic resetCounters;

// Count all ticks (4 ticks per turn). 
always_ff @(posedge inA, posedge resetCounters)
begin
	if(resetCounters) tickcount1 <= 16'b0;
	else if(inB) tickcount1 <= tickcount1 - 16'b1;
	else tickcount1 <= tickcount1 + 16'b1;
end
always_ff @(posedge inB, posedge resetCounters)
begin
	if(resetCounters) tickcount2 <= 16'b0;
	else if(inA) tickcount2 <= tickcount2 + 16'b1;
	else tickcount2 <= tickcount2 - 16'b1;
end
always_ff @(negedge inA, posedge resetCounters)
begin
	if(resetCounters) tickcount3 <= 16'b0;
	else if(inB) tickcount3 <= tickcount3 + 16'b1;
	else tickcount3 <= tickcount3 - 16'b1;
end
always_ff @(negedge inB, posedge resetCounters)
begin
	if(resetCounters) tickcount4 <= 16'b0;
	else if(inA) tickcount4 <= tickcount4 - 16'b1;
	else tickcount4 <= tickcount4 + 16'b1;
end

// Reset, count clock cycles and transmit tickcount every 1 250 000 cycles (= 25 ms)
always_ff @(posedge clk, posedge reset)
	if(reset)
	begin
		clkcount <= 32'b0;
		resetCounters <= 1;
		speed <= 16'b0;
	end
	else 
		if(clkcount == 32'd1250000)
		begin
			speed 	<= tickcount1 + tickcount2 + tickcount3 + tickcount4;
			resetCounters <= 1;
			clkcount <= 32'b0;
		end
		else
		begin
			clkcount <= clkcount + 32'b1;
			resetCounters <= 0;
		end

endmodule 