`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, a, b, c, d, e, f, g, h, i, j, k, l);
input Clk, Rst, Start;
output reg Done;
input signed [15:0] a, b, c, d, e, f, g, h;
output reg signed [15:0] i, j, k, l;
reg [2:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4;

always@(State, Start, a, b, c, d, e, f, g, h)begin
case (State)

sWait: begin
Done = 0;
i = 0;
j = 0;
k = 0;
l = 0;
if (Start == 1)
NextState <= S1;
else
NextState <= sWait;
end

S1: begin
i = a * b;
j = c * d;
k = e * f;
l = g * h;
NextState <= S2;
end

S2: begin
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
i= 0;
j= 0;
k= 0;
l = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule