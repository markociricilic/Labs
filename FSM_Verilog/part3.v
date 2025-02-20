module part3(Clock, Resetn, Go, Divisor, Dividend, Quotient, Remainder);

	input Clock;
	input Resetn;
	input Go;
	
	input [3:0] Dividend;
	input [3:0] Divisor;
	
	output reg [3:0] Quotient;
	output reg [3:0] Remainder;
	
	reg [3:0] var;
	
	always @(posedge Clock)
		begin
			if (Resetn == 0)
				begin
					Quotient = 4'b0000;
					Remainder = 4'b0000;
				end
			else if (Go)
				begin
					Remainder = Dividend % Divisor;
					var = Dividend - Remainder;
					Quotient = var / Divisor;
				end
		end
endmodule
