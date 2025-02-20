//
// This is the template for Part 3 of Lab 7.
//
// Paul Chow
// November 2021
//

// iColour is the colour for the box
//
// oX, oY, oColour and oPlot should be wired to the appropriate ports on the VGA controller
//

// Some constants are set as parameters to accommodate the different implementations
// X_SCREENSIZE, Y_SCREENSIZE are the dimensions of the screen
//       Default is 160 x 120, which is size for fake_fpga and baseline for the DE1_SoC vga controller
// CLOCKS_PER_SECOND should be the frequency of the clock being used.

module part3(iColour,iResetn,iClock,oX,oY,oColour,oPlot);
   input wire [2:0] iColour;
   input wire       iResetn;
   input wire       iClock;
   output wire [7:0] oX;         // VGA pixel coordinates
   output wire [6:0] oY;
   
   output wire [2:0] oColour;     // VGA pixel colour (0-7)
   output wire       oPlot;       // Pixel drawn enable

   parameter
     X_SCREENSIZE = 160,  // X screen width for starting resolution and fake_fpga
     Y_SCREENSIZE = 120,  // Y screen height for starting resolution and fake_fpga
     CLOCKS_PER_SECOND = 5000, // 5 KHZ for fake_fpga
     X_BOXSIZE = 8'd4,   // Box X dimension
     Y_BOXSIZE = 7'd4,   // Box Y dimension
     X_MAX = X_SCREENSIZE - 1 - X_BOXSIZE, // 0-based and account for box width
     Y_MAX = Y_SCREENSIZE - 1 - Y_BOXSIZE,
     PULSES_PER_SIXTIETH_SECOND = CLOCKS_PER_SECOND / 60;

   //
   // Your code goes here
   //

   wire [7:0]XC;
   wire [6:0]YC;
   wire DC;
   wire [3:0]FC;
   wire ControlA_EraseBox, ControlB_iPlotBox,ControlD_Wait;

   CounterX X(iClock, iResetn, ControlC_Update, oX,X_MAX, XC);
   CounterY Y(iClock, iResetn, ControlC_Update,oY, Y_MAX, YC);
   DelayCounter Delay_Counter(iClock, iResetn, PULSES_PER_SIXTIETH_SECOND, DC);
   FrameCounter Frame_Counter(iClock, iResetn, ControlD_Wait, PULSES_PER_SIXTIETH_SECOND,FC);
   controlPath CP(iResetn, iClock, DC,FC, ControlA_EraseBox, ControlB_iPlotBox,ControlC_Update,ControlD_Wait, oPlot);
   datapath DP(iClock, iResetn, iColour,XC,YC, ControlA_EraseBox, ControlB_iPlotBox, oX, oY, oColour);

endmodule // part3



module controlPath(input iResetn, input iClock, input DelayCounter,input [3:0]FrameCounter, output reg controlA, output reg controlB,output reg controlC,output reg controlD,output reg oPlot);

  reg [2:0]current_state, next_state;
  wire [3:0]count;
  wire [3:0]count2;
  reg en;
wire done,done2;
  
  localparam    reset = 3'd0,
                DRAW_BOX = 3'd1,
                DRAW_BOX_WAIT = 3'd2,
    TRANSITION = 3'd3,
                ERASE_BOX = 3'd4,
    ERASE_BOX_WAIT = 3'd5,
                UPDATE = 3'd6;

  bit_counter C1(iClock, iResetn,controlB, count, done);
  bit_counter C2(iClock, iResetn,en,count2, done2);

//State Table
always @(*)begin
  case(current_state)
    reset: next_state = iResetn ? DRAW_BOX: reset;
    DRAW_BOX: next_state = (done) ? DRAW_BOX_WAIT : DRAW_BOX;
    DRAW_BOX_WAIT: next_state = (FrameCounter == 4'd15) ? TRANSITION : DRAW_BOX_WAIT ;
    TRANSITION: next_state = (FrameCounter == 4'd15) ? TRANSITION : ERASE_BOX ;
    ERASE_BOX: next_state = (done2) ? ERASE_BOX_WAIT : ERASE_BOX;
    ERASE_BOX_WAIT: next_state = (done2) ? ERASE_BOX_WAIT:UPDATE;
    UPDATE: next_state = DRAW_BOX;
    default: next_state = reset;
  endcase
end


//Output Logic
always @(*)begin

  controlA = 1'b0;
  controlB = 1'b0;
controlC = 1'b0;
  controlD = 1'b0;
  oPlot = 1'b0;

    case(current_state)
      DRAW_BOX: begin
        controlB = 1'b1;
        oPlot = 1'b1;
        end
      DRAW_BOX_WAIT: begin
    controlD = 1'b1;
    controlA = 1'b0;
    end
	TRANSITION:begin
	controlD = 1'b1;
    	controlA = 1'b0;
    	end
      ERASE_BOX: begin
        en = 1'b1;
        controlA = 1'b1;
        oPlot = 1'b1;
        end
	ERASE_BOX_WAIT: begin
	en = 1'b1;
        controlA = 1'b1;
        oPlot = 1'b1;
	end
      UPDATE: begin
        controlC = 1'b1;
        end
    endcase
  end

//State Register
always @(posedge iClock,negedge iResetn)
  begin
    if(iResetn == 0)
      current_state <= DRAW_BOX;
    else 
      current_state <= next_state;
  end

endmodule

module datapath( input iClock, input iResetn, input [2:0]iColour, input[7:0]x_loc, input [6:0]y_loc, input controlA_eraseBox, input controlB_iPlotBox,
 output reg [7:0] oX, output reg [6:0] oY, output reg [2:0] oColour);

  //wire [7:0]X_count;
  //wire [6:0]Y_count;
  wire [3:0]c;
  wire [3:0]c2;
  reg [6:0]x_out;
  reg [6:0]y_out;
  reg [6:0]x_out_erase;
  reg [6:0]y_out_erase;
  reg [2:0]colour_out;
wire done;
  //wire draw = 0;
  
  bit_counter C1(iClock, iResetn, controlB_iPlotBox, c, done);
  bit_counter C2(iClock, iResetn, controlA_eraseBox, c2, done);
  assign oColour = iColour;
//X_Y_Colour_register
always@(*)begin
    x_out <= x_loc;
    y_out <= y_loc;
    x_out_erase <= x_loc;
    y_out_erase <= y_loc;
    //colour_out <= 3'b000;
  if(iResetn == 0) begin
    x_out <= 8'b00000000;
    y_out <= 7'b0000000;
    end
  else begin
  if(controlB_iPlotBox == 1)
    colour_out <= iColour;
    //colour_out <= 3'b011;
end
end

//Output result register
always@(*) begin
  if(iResetn == 0) begin
    //oColour <= 3'b000;
    //oColour <= colour_out;
    oX <= 8'b00000000;
    oY <= 7'b0000000;
  end
else if(controlA_eraseBox == 1)begin
      oX <= {0,x_out + c2[1:0]};
      oY <= y_out + c2[3:2];
      oColour <= 3'b000;
  end
  else if(controlB_iPlotBox == 1) begin
      oX <= {0,x_out + c[1:0]};
      oY <= y_out + c[3:2];
      oColour <= colour_out;
    end
  else begin
    oX <= oX;
    oY <= oY;
    oColour <= oColour;
  end
  end
  //assign update = (c2 == 4'b1111) ? 1 : 0;
endmodule


module bit_counter(input clock, resetn, En, output reg [3:0]Q = 0, output done);
  
  always @(negedge resetn, posedge clock)begin
    if(resetn == 0)
      Q <= 0;
    else if(Q == 15)
      Q <= 0;
    else if(En == 1)
      Q <= Q +1;
  end
assign done = (Q == 15) ? 1: 0;
endmodule

module DelayCounter(input clock, resetn, [10:0]PULSES_PER_SIXTIETH_SECOND, output Enable);
  
  reg [4:0] Q;
  always @(negedge resetn, posedge clock)begin
    if(resetn == 0)
      Q <= PULSES_PER_SIXTIETH_SECOND;
    else if(Q == 0)
      Q <= PULSES_PER_SIXTIETH_SECOND;
    else 
      Q <= Q - 1;
  end
  assign Enable = (Q == 0)?1:0;
endmodule

module FrameCounter(input clock, resetn, enable,[10:0]PULSES_PER_SIXTIETH_SECOND, output reg [3:0] Q = 0);

  wire en;
  DelayCounter Counter(clock, resetn, PULSES_PER_SIXTIETH_SECOND, en);
  always @(negedge resetn, posedge clock)begin
    if(resetn == 0)
      Q <= 0;
    else if(Q == 15 && en == 1)
      Q <= 0;
    else if(en == 1 && enable == 1)
      Q <= Q + 1;
  end
endmodule

module CounterX(input clock, resetn,update, input [7:0] x_int, [7:0]X_MA, output reg [7:0]Q);
  reg DirH = 1;
  reg [7:0]x_out;

always@(*)begin
    if(resetn == 0)
      Q <= 0;
    else
      x_out <= x_int;
    end

  always@(negedge resetn, posedge clock)begin
  //x_out <= x_loc;
    if(resetn == 0)
      Q <= 0;
    else if(DirH == 0 && update == 1) begin
      if(x_out - 4 < 0)begin
        DirH <= 1;
        Q <= x_out + 1;
      end
      else
        Q <= x_out - 1;
      end
    else if(DirH == 1 && update == 1) begin
      if(x_out + 4 > X_MA)begin
        DirH <= 0;
        Q <= x_out - 1;
      end
      else 
        Q <= x_out + 1;
      end
  end
endmodule

module CounterY(input clock, resetn, update,input [6:0] y_int, [6:0]Y_MA,output reg [6:0]Q);
  reg DirY = 0;
  reg [6:0]y_out;

  always@(*)begin
    if(resetn == 0)
      Q <= 0;
    else
      y_out <= y_int;
    end
  always@(negedge resetn, posedge clock)begin
    if(resetn == 0)
      Q <= 0;
    else if(DirY == 1 && update == 1)begin
      if(y_out - 4 < 0)begin
        DirY <= 1;
        Q <= y_out + 1;
      end
      else
        Q <= y_out - 1;
      end
    else if(DirY == 0 && update == 1)begin
      if(y_out + 4 >= Y_MA)begin
        DirY <= 0;
        Q <= y_out - 1;
  end
      else 
        Q <= y_out + 1;
      end
  end
endmodule

