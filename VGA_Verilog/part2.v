//
// This is the template for Part 2 of Lab 7.
//
// Paul Chow
// November 2021
//

module part2(iResetn, iPlotBox, iBlack, iColour, iLoadX, iXY_Coord, iClock, oX, oY, oColour, oPlot);
   parameter X_SCREEN_PIXELS = 8'd160;
   parameter Y_SCREEN_PIXELS = 7'd120;
   
   input wire iResetn, iPlotBox, iBlack, iLoadX;
   input wire [2:0] iColour;
   input wire [6:0] iXY_Coord;
   input wire 	     iClock;
	
   output wire [7:0] oX;         // VGA pixel coordinates
   output wire [6:0] oY;
   
   output wire [2:0] oColour;     // VGA pixel colour (0-7)
   output wire 	     oPlot;       // Pixel draw enable                  
	
	wire ld_x, ld_y, dataOut;
   wire [3:0] counter;
	
	control u1(.clk(iClock), .resetn(iResetn), .go(iLoadX), .plotBox(iPlotBox), .ld_x(ld_x), .ld_y(ld_y), .dataOut(dataOut), .plot(oPlot), .counter(counter), .BLACKSCREEN(iBlack));
	
   datapath u2(.clk(iClock), .resetn(iResetn), .alu_select_xy_coord(iXY_Coord), .colour(iColour), .ld_x(ld_x), .ld_y(ld_y), .dataIn(dataOut), .x_coord(oX), 
					.y_coord(oY), .colour_output(oColour), .counter(counter));
		
endmodule


module control(input clk, resetn, go, plotBox, BLACKSCREEN,
					input [3:0] counter,
					output reg ld_x, ld_y, dataOut, plot
					);

	reg [2:0] current_state, next_state; 

	localparam 	S_LOAD_X 				= 3'd0,
					S_LOAD_X_WAIT 			= 3'd1, 
					S_LOAD_Y 				= 3'd2,
					S_LOAD_Y_WAIT 			= 3'd3,
					S_CYCLE_PLOT 			= 3'd4,
					S_CYCLE_PLOT_WAIT 	= 3'd5;
					
	 // Next state logic aka our state table					
	 always@(*)
		 begin: state_table 
				case (current_state)
								S_LOAD_X: 		next_state = go ? S_LOAD_X_WAIT : S_LOAD_X; 
								S_LOAD_X_WAIT: next_state = go ? S_LOAD_X_WAIT : S_LOAD_Y;
								S_LOAD_Y: 		next_state = plotBox ? S_LOAD_Y_WAIT : S_LOAD_Y; 
								S_LOAD_Y_WAIT: next_state = plotBox ? S_LOAD_Y_WAIT : S_CYCLE_PLOT;
								
								S_CYCLE_PLOT: 
									begin
										if(counter == 4'd15)
											next_state = S_CYCLE_PLOT_WAIT;
										else
											next_state = S_CYCLE_PLOT;
									end
								S_CYCLE_PLOT_WAIT: next_state = S_LOAD_X;
					default: next_state = S_LOAD_X;
			  endcase
		 end // state_table
	
	always @(*)
		begin: enable_signals
		// By default make all our signals 0
			ld_x = 1'b0;
			ld_y = 1'b0;
			dataOut = 1'b0;

        case (current_state)
            S_LOAD_X: begin
                ld_x = 1'b1; 
					 plot = 1'b0;
					 end
            S_LOAD_Y: begin
                ld_y = 1'b1;
					 plot = 1'b0;
					 end
            S_CYCLE_PLOT: begin
                dataOut = 1'b1; 
					 plot = 1'b1;
					 end
				 // default: don't need default since we already made sure all of our outputs were assigned a value at the start of the always block
			endcase
		end // enable_signals

		always@(posedge clk) 
			begin: reset
				if(BLACKSCREEN) 
					begin

					end
			end
	
	// current_state registers
	always@(posedge clk)
		begin: state_FFs
			if(!resetn)
				current_state <= S_LOAD_X;
			else
				current_state <= next_state;
		end // state_FFS

endmodule


module datapath(input clk, resetn, ld_x, ld_y, dataIn,    
					input [2:0] colour,
					input [6:0] alu_select_xy_coord,
					
					output reg [3:0] counter,
					output reg [7:0] x_coord,
					output reg [6:0] y_coord,
					output reg [2:0] colour_output
					);
	
	reg [7:0] x_reg;
	reg [6:0] y_reg;
	reg [2:0] colour_reg;
	
	reg [3:0] count;
	
	always @(posedge clk)
		begin
		  if(!resetn)
				count <= 4'b0000;
		  else if(count == 4'b1111)
				count <= 4'b0000;
		  else if(dataIn)
				count <= count + 1;
		end
		
		assign counter = count;

	always@(posedge clk)
		begin
			if(!resetn)
				begin
					x_coord <= 8'b00000000; 
					y_coord <= 7'b0000000; 
					colour_output <= 3'b000;
				end
			else if (ld_x) 
				begin
					x_reg <= {0, alu_select_xy_coord};
				end
			else if (ld_y) 
				begin
					y_reg <= alu_select_xy_coord;
					colour_reg <= colour;
				end
			else if (dataIn)
				begin
					x_coord <= x_reg + counter[1:0];
					y_coord <= y_reg + counter[3:2];
					colour_output <= colour_reg;
				end
		end
		
endmodule
