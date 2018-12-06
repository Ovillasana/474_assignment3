`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, a, b, c, d, e, i);
input Clk, Rst, Start;
output reg Done;
input signed [15:0] a, b, c, d, e;
output reg signed [15:0] i;
reg signed [15:0] f, g, h;
reg [4:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4, S4 = 5, S5 = 6, S6 = 7, S7 = 8, S8 = 9, S9 = 10, S10 = 11;

always@(State, Start, a, b, c, d, e)begin
case (State)

sWait: begin
Done = 0;
i = 0;
f = 0;
g = 0;
h = 0;
if (Start == 1)
NextState <= S1;
else
NextState <= sWait;
end

S1: begin
f = a * b;
NextState <= S2;
end

S2: begin
NextState <= S3;
end

S3: begin
NextState <= S4;
end

S4: begin
g = f * c;
NextState <= S5;
end

S5: begin
NextState <= S6;
end

S6: begin
h = g * d;
NextState <= S7;
end

S7: begin
NextState <= S8;
end

S8: begin
i = h * e;
NextState <= S9;
end

S9: begin
NextState <= sFinal;
end

sFinal: begin
Done = 1;
NextState <= sWait;
end

endcase
end

always@(posedge Clk)begin
if (Rst == 1) begin
Done = 0;
i = 0;
f= 0;
g= 0;
h = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule