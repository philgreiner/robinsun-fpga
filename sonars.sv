/* 
* Controls 3 sonars, and sends regular pulses to determine the direction and distance of an obstacle
*/
module sonars(
input  logic clk, reset, S1, S2, S3, // Three signals coming from sonars
output logic T1, T2, T3,				 // Trigger signals
output logic [19:0] R1, R2, R3); 	 // output signals (in number of clock ticks)

logic [19:0] triggerDelay, count1, count2, count3;
logic [8:0]  triggerCount;
logic [2:0]  triggers;
logic [1:0]  active;

assign {T3, T2, T1} = triggers;

always_ff @(posedge clk, posedge reset)
begin
	if(reset)
	begin
		triggerDelay <= 20'b0;
		triggerCount <= 9'b0;
		triggers	    <= 3'b0;
		active		 <= 2'b0;
		R1				 <= 20'b0;
		R2				 <= 20'b0;
		R3				 <= 20'b0;
	end
	else
	begin
		if(triggerDelay == 20'd882400) // Corresponds to the number of clock cycles necessary for 
		begin										// the signal to travel 2x3m (6 [m] / 340 [m/s] * 50e6 [1/s])
			case(active)
				2'b10: begin 				// If sonar 3 was active, save the result and activate sonar 1
							R3 <= count3; 
							triggers <= 3'b001;
							active <= 2'b00;   
						 end
				2'b00: begin 				// If sonar 1 was active, save the result and activate sonar 2
							R1 <= count1; 
							triggers <= 3'b010;
							active <= 2'b01;   
						 end
				2'b01: begin				// If sonar 2 was active, save the result and activate sonar 3
							R2 <= count2; 
							triggers <= 3'b100;
							active <= 2'b10;
						 end
			endcase
			triggerDelay <= 20'b0;		// Reset delay and triggercount when sending a new signal
			triggerCount <= 9'b0;
		end
		else
		begin
			triggerCount <= triggerCount + 9'b1;
			triggerDelay <= triggerDelay + 20'b1;
			if(triggerCount == 9'd500)		// corresponds to the number of clock cycles to have a trigger of 10us
				triggers <= 3'b0;
		end
	end
end

always_ff @(posedge clk, posedge reset)
begin 
	if(reset)
	begin
		count1 <= 20'b0;
		count2 <= 20'b0;
		count3 <= 20'b0;
	end
	else
	begin
		case(active)
			2'b00: if(T1) 		count1 <= 20'b0;					// Reset count while trigger is sent
						else if(S1) count1 <= count1 + 20'b1;	// Count while signal is high
								else count1 <= count1;				// Keep the result after signal end
			2'b01: if(T2) 		count2 <= 20'b0;
						else if(S2) count2 <= count2 + 20'b1;
								else count2 <= count2;
			2'b10: if(T3) 		count3 <= 20'b0;
						else if(S3) count3 <= count3 + 20'b1;
								else count3 <= count3; 
		endcase
	end
end

endmodule 