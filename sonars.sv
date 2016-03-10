/* 
* Controls 3 sonars, and sends regular pulses to determine the direction and distance of an obstacle
*/
module sonars(
input  logic clk, reset, S1, S2, S3, // Three signals coming from sonars
output logic T1, T2, T3,	
output logic [19:0] R1, R2, R3);

logic [19:0] triggerDelay, count1, count2, count3;
logic [4:0]  triggerCount;
logic [2:0]  triggers;
logic [1:0]  active;

assign {T1, T2, T3} = triggers;

always_ff @(posedge clk, posedge reset)
begin
	if(reset)
	begin
		triggerDelay <= 20'b0;
		triggerCount <= 5'b0;
		triggers	    <= 3'b0;
		active		 <= 2'b0;
	end
	else
	begin
		if(triggerDelay == 20'd882400) // send a new pulse on the next sonar
		begin
			case(active)
				2'b10: begin 
							R3 <= count3; 
							triggers <= 3'b001;
							active <= 2'b00;   
						 end
				2'b00: begin 
							R1 <= count1; 
							triggers <= 3'b010;
							active <= 2'b01;   
						 end
				2'b01: begin
							R2 <= count2; 
							triggers <= 3'b100;
							active <= 2'b10;
						 end
			endcase
			triggerDelay <= 20'b0;
			triggerCount <= 5'b0;
		end
		else
		begin
			triggerCount <= triggerCount + 5'b1;
			triggerDelay <= triggerDelay + 20'b1;
			if(triggerCount == 5'd25)
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
			2'b00: if(T1) 		count1 <= 20'b0;
						else count1 <= count1 + 20'b1;
			2'b01: if(T2) 		count2 <= 20'b0;
						else count2 <= count2 + 20'b1;
			2'b10: if(T3) 		count3 <= 20'b0;
						else count3 <= count3 + 20'b1;
		endcase
	end
end

endmodule 