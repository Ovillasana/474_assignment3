`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, a, b, c, d, e, f, g, j, l);
input Clk, Rst, Start;
output reg Done;
input signed [15:0] a, b, c, d, e, f, g;
reg signed [15:0] h, i, k;
output reg signed [15:0] j, l;
reg [3:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4, S4 = 5, S5 = 6;

always@(State, Start, a, b, c, d, e, f, g)begin
case (State)

sWait: begin
Done = 0;
h = 0;
i = 0;
k = 0;
j = 0;
l = 0;
if (Start == 1)
NextState <= S1;
else
NextState <= sWait;
end

S1: begin
k = e * f;
NextState <= S2;
end

S2: begin
h = a + b;
NextState <= S3;
end

S3: begin
i = h + c ;
l = k * g;
NextState <= S4;
end

S4: begin
j = i + d ;
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
h= 0;
i= 0;
k = 0;
j= 0;
l = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule