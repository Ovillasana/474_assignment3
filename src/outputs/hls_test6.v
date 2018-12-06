`timescale 1ns / 1ps
module HLSM (Clk, Rst, Start, Done, a, b, c, d, e, f, g, h, i, j, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii, final);
input Clk, Rst, Start;
output reg Done;
input [32:0] a, b, c, d, e, f, g, h, i, j, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii;
output reg [32:0] final;
reg [32:0] t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31, t32, t33;
reg [6:0] State, NextState;

parameter sWait = 0, sFinal = 1;
parameter S1 = 2, S2 = 3, S3 = 4, S4 = 5, S5 = 6, S6 = 7, S7 = 8, S8 = 9, S9 = 10, S10 = 11, S11 = 12, S12 = 13, S13 = 14, S14 = 15, S15 = 16, S16 = 17, S17 = 18, S18 = 19, S19 = 20, S20 = 21, S21 = 22, S22 = 23, S23 = 24, S24 = 25, S25 = 26, S26 = 27, S27 = 28, S28 = 29, S29 = 30, S30 = 31, S31 = 32, S32 = 33, S33 = 34, S34 = 35, S35 = 36;

always@(State, Start, a, b, c, d, e, f, g, h, i, j, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, aa, bb, cc, dd, ee, ff, gg, hh, ii)begin
case (State)

sWait: begin
Done = 0;
final = 0;
t1 = 0;
t2 = 0;
t3 = 0;
t4 = 0;
t5 = 0;
t6 = 0;
t7 = 0;
t8 = 0;
t9 = 0;
t10 = 0;
t11 = 0;
t12 = 0;
t13 = 0;
t14 = 0;
t15 = 0;
t16 = 0;
t17 = 0;
t18 = 0;
t19 = 0;
t20 = 0;
t21 = 0;
t22 = 0;
t23 = 0;
t24 = 0;
t25 = 0;
t26 = 0;
t27 = 0;
t28 = 0;
t29 = 0;
t30 = 0;
t31 = 0;
t32 = 0;
t33 = 0;
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
t2 = t1 + c;
NextState <= S3;
end

S3: begin
t3 = t2 + d;
NextState <= S4;
end

S4: begin
t4 = t3 + e;
NextState <= S5;
end

S5: begin
t5 = t4 + f;
NextState <= S6;
end

S6: begin
t6 = t5 + g;
NextState <= S7;
end

S7: begin
t7 = t6 + h;
NextState <= S8;
end

S8: begin
t8 = t7 + i;
NextState <= S9;
end

S9: begin
t9 = t8 + j;
NextState <= S10;
end

S10: begin
t10 = t9 + l;
NextState <= S11;
end

S11: begin
t11 = t10 + m;
NextState <= S12;
end

S12: begin
t12 = t11 + n;
NextState <= S13;
end

S13: begin
t13 = t12 + o;
NextState <= S14;
end

S14: begin
t14 = t13 + p;
NextState <= S15;
end

S15: begin
t15 = t14 + q;
NextState <= S16;
end

S16: begin
t16 = t15 + r;
NextState <= S17;
end

S17: begin
t17 = t16 + s;
NextState <= S18;
end

S18: begin
t18 = t17 + t;
NextState <= S19;
end

S19: begin
t19 = t18 + u;
NextState <= S20;
end

S20: begin
t20 = t19 + v;
NextState <= S21;
end

S21: begin
t21 = t20 + w;
NextState <= S22;
end

S22: begin
t22 = t21 + x;
NextState <= S23;
end

S23: begin
t23 = t22 + y;
NextState <= S24;
end

S24: begin
t24 = t23 + z;
NextState <= S25;
end

S25: begin
t25 = t24 + aa;
NextState <= S26;
end

S26: begin
t26 = t25 + bb;
NextState <= S27;
end

S27: begin
t27 = t26 + cc;
NextState <= S28;
end

S28: begin
t28 = t27 + dd;
NextState <= S29;
end

S29: begin
t29 = t28 + ee;
NextState <= S30;
end

S30: begin
t30 = t29 + ff;
NextState <= S31;
end

S31: begin
t31 = t30 + gg;
NextState <= S32;
end

S32: begin
t32 = t31 + hh;
NextState <= S33;
end

S33: begin
t33 = t32 + ii;
NextState <= S34;
end

S34: begin
final = t33 + t1;
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
final = 0;
t1= 0;
t2= 0;
t3= 0;
t4= 0;
t5= 0;
t6= 0;
t7= 0;
t8= 0;
t9= 0;
t10= 0;
t11= 0;
t12= 0;
t13= 0;
t14= 0;
t15= 0;
t16= 0;
t17= 0;
t18= 0;
t19= 0;
t20= 0;
t21= 0;
t22= 0;
t23= 0;
t24= 0;
t25= 0;
t26= 0;
t27= 0;
t28= 0;
t29= 0;
t30= 0;
t31= 0;
t32= 0;
t33 = 0;
State <= sWait;
end
else
State <= NextState;
end

endmodule