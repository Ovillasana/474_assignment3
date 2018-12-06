`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, a, b, c, z, x);
input Clk, Rst, Start;
output reg Done;
input signed [32:0] a, b, c;
output reg signed [32:0] z, x;
reg signed [32:0] d, e, f, g, h;
reg dLTe, dEQe;
reg [3:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4, S4 = 5, S5 = 6, S6 = 7, S7 = 8;

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
h = 0;
dLTe = 0;
dEQe = 0;
if (Start == 1)
NextState <= S1;
else
NextState <= sWait;
end

S1: begin
d = a + b;
NextState <= S2;
end

S2: begin
e = a + c;
NextState <= S3;
end

S3: begin
dLTe = d < e;
NextState <= S4;
end

S4: begin
f = a - b  ;
dEQe = d == e;
g = dLTe ? d : e ;
NextState <= S5;
end

S5: begin
h = dEQe ? g : f ;
NextState <= S6;
end

S6: begin
x = g << dLTe;
z = h >> dEQe;
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
z= 0;
x = 0;
d= 0;
e= 0;
f= 0;
g= 0;
h = 0;
dLTe= 0;
dEQe = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule