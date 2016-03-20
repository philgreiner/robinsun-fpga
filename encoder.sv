/* 
* Uses the entries (A and B) from a motor encoder compute:
*		speed [15:0] --> Rotation speed (ticks per 1 ms)
*		direction [7:0]   --> Direction of rotation (1 = forward; 0 = reverse)
*/
module encoder(
input  logic clk, reset, inA, inB,
output logic [7:0] direction,
output logic [15:0] speed);

logic [15:0] tickcount1, tickcount2, tickcount3, tickcount4;
logic [31:0] clkcount;
logic resetCounters;

// Count all ticks (4 ticks per turn). 
always_ff @(posedge inA, posedge resetCounters)
begin
	if(resetCounters) tickcount1 <= 16'b0;
	else tickcount1 <= tickcount1 + 16'b1;
end
always_ff @(posedge inB, posedge resetCounters)
begin
	if(resetCounters) tickcount2 <= 16'b0;
	else tickcount2 <= tickcount2 + 16'b1;
end
always_ff @(negedge inA, posedge resetCounters)
begin
	if(resetCounters) tickcount3 <= 16'b0;
	else tickcount3 <= tickcount3 + 16'b1;
end
always_ff @(negedge inB, posedge resetCounters)
begin
	if(resetCounters) tickcount4 <= 16'b0;
	else tickcount4 <= tickcount4 + 16'b1;
end

/* Check direction of rotation
*
* In forward direction of rotation, the signals are:
*           ________         ________         ________         ________        
* A _______|        |_______|        |_______|        |_______|        |___
*       ________         ________         ________         ________        
* B ___|        |_______|        |_______|        |_______|        |_______
*
*/
always_ff @(posedge inA)
begin
	if(inB)  direction <= {7'b0, 1'b1};
	else 		direction <= 8'b0;
end

// Reset, count clock cycles and transmit tickcount every 5 000 000 cycles (= 100 ms)
always_ff @(posedge clk, posedge reset)
	if(reset)
	begin
		clkcount <= 32'b0;
		resetCounters <= 1;
		speed <= 16'b0;
	end
	else 
		if(clkcount == 32'd5000000)
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