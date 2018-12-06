`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, a, b, c, z, x);
input Clk, Rst, Start;
output reg Done;
input [15:0] a, b, c;
output reg [7:0] z;
output reg [15:0] x;
reg [7:0] d, e, f, g;
reg [3:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4, S4 = 5, S5 = 6;

always@(State, Start, a, b, c)begin
case (State)

sWait: begin
Done = 0;
z = 0;
x = 0;
d = 0;
e = 0;
f = 0;
g = 0;
if (Start == 1)
NextState <= S1;
else
NextState <= sWait;
end

S1: begin
d = a + b;
f = a * c;
NextState <= S2;
end

S2: begin
e = a + c;
NextState <= S3;
end

S3: begin
g = d > e;
NextState <= S4;
end

S4: begin
z = g ? d : e;
x = f - d;
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
z = 0;
x = 0;
d= 0;
e= 0;
f= 0;
g = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule