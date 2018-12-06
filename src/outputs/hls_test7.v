`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, u, x, y, dx, a, three, u1, x1, y1, c);
input Clk, Rst, Start;
output reg Done;
input signed [32:0] u, x, y, dx, a, three;
output reg signed [32:0] u1, x1, y1, c;
reg signed [32:0] t1, t2, t3, t4, t5, t6, t7, vx1;
reg [4:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4, S4 = 5, S5 = 6, S6 = 7, S7 = 8, S8 = 9, S9 = 10;

always@(State, Start, u, x, y, dx, a, three)begin
case (State)

sWait: begin
Done = 0;
u1 = 0;
x1 = 0;
y1 = 0;
c = 0;
t1 = 0;
t2 = 0;
t3 = 0;
t4 = 0;
t5 = 0;
t6 = 0;
t7 = 0;
vx1 = 0;
if (Start == 1)
NextState <= S1;
else
NextState <= sWait;
end

S1: begin
t1 = three * x;
NextState <= S2;
end

S2: begin
NextState <= S3;
end

S3: begin
t2 = u * dx;
NextState <= S4;
end

S4: begin
t5 = three * y;
NextState <= S5;
end

S5: begin
t3 = t1 * t2;
NextState <= S6;
end

S6: begin
t6 = t5 * dx;
t7 = u * dx;
NextState <= S7;
end

S7: begin
x1 = x + dx;
vx1 = x + dx;
t4 = u - t3;
NextState <= S8;
end

S8: begin
u1 = t4 - t6;
y1 = y + t7;
c = vx1 < a;
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
u1= 0;
x1= 0;
y1= 0;
c = 0;
t1= 0;
t2= 0;
t3= 0;
t4= 0;
t5= 0;
t6= 0;
t7= 0;
vx1 = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule