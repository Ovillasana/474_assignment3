`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, a, b, c, d, e, f, g, h, sa, avg);
input Clk, Rst, Start;
output reg Done;
input [32:0] a, b, c, d, e, f, g, h, sa;
output reg [32:0] avg;
reg [32:0] t1, t2, t3, t4, t5, t6, t7, t7div2, t7div4;
reg [4:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4, S4 = 5, S5 = 6, S6 = 7, S7 = 8, S8 = 9, S9 = 10, S10 = 11, S11 = 12, S12 = 13;

always@(State, Start, a, b, c, d, e, f, g, h, sa)begin
case (State)

sWait: begin
Done = 0;
avg = 0;
t1 = 0;
t2 = 0;
t3 = 0;
t4 = 0;
t5 = 0;
t6 = 0;
t7 = 0;
t7div2 = 0;
t7div4 = 0;
if (Start == 1)
NextState <= S1;
else
NextState <= sWait;
end

S1: begin
t1 = a + b;
NextState <= S2;
end

S2: begin
NextState <= S3;
end

S3: begin
t2 = t1 + c ;
NextState <= S4;
end

S4: begin
t3 = t2 + d ;
NextState <= S5;
end

S5: begin
t4 = t3 + e ;
NextState <= S6;
end

S6: begin
t5 = t4 + f ;
NextState <= S7;
end

S7: begin
t6 = t5 + g ;
NextState <= S8;
end

S8: begin
t7 = t6 + h ;
NextState <= S9;
end

S9: begin
t7div2 = t7 >> sa;
NextState <= S10;
end

S10: begin
t7div4 = t7div2 >> sa ;
NextState <= S11;
end

S11: begin
avg = t7div4 >> sa;
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
avg = 0;
t1= 0;
t2= 0;
t3= 0;
t4= 0;
t5= 0;
t6= 0;
t7= 0;
t7div2= 0;
t7div4 = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule