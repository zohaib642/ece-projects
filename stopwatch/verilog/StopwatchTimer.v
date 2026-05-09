`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/16/2026 06:11:56 PM
// Design Name: 
// Module Name: StopwatchTimer
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module StopwatchTimer(
    input clk,
    input reset,
    input start_stop,
    input [1:0] mode,
    input [7:0] time_value,
    output [6:0] sseg,
    output dp,
    output reg [3:0] an
);

    // Slow clock
    wire slow_clk;
    clk_div cd(.clk(clk), .reset(reset_clean), .clk_out(slow_clk));

    // Edge detection for start_stop and reset to fix debouncing
    reg ss_prev;
    reg running;
    reg reset_prev;
    reg reset_clean;
    always @(posedge clk) begin
        ss_prev <= start_stop;
        if (reset_clean) begin
            running <= 0;
            ss_prev <= 0;
        end
        else if (start_stop && !ss_prev)
            running <= ~running;
            
        reset_prev <= reset;
        reset_clean <= reset && !reset_prev;
    end

    // Updating the digits
    reg [3:0] d0, d1, d2, d3;

    always @(posedge clk) begin
        if (reset_clean) begin
            case (mode)
                2'd0: begin d0<=0; d1<=0; d2<=0; d3<=0; end
                2'd1: begin d0<=9; d1<=9; d2<=9; d3<=9; end
                2'd2: begin d0<=0; d1<=0; d2<=(time_value[3:0]>9)? 9:time_value[3:0]; d3<=(time_value[7:4]>9)? 9:time_value[7:4]; end
                2'd3: begin d0<=0; d1<=0; d2<=(time_value[3:0]>9)? 9:time_value[3:0]; d3<=(time_value[7:4]>9)? 9:time_value[7:4]; end
                default: begin d0<=0; d1<=0; d2<=0; d3<=0; end
            endcase
        end
        else if (slow_clk && running) begin
            if (mode == 2'd0 || mode == 2'd2) begin
                if (d0==9 && d1==9 && d2==9 && d3==9) begin
                end
                else if (d0 == 9) begin
                    d0 <= 0;
                    if (d1 == 9) begin
                        d1 <= 0;
                        if (d2 == 9) begin
                            d2 <= 0;
                            d3 <= d3 + 1;
                        end else d2 <= d2 + 1;
                    end else d1 <= d1 + 1;
                end else d0 <= d0 + 1;
            end
            else begin
                if (d0==0 && d1==0 && d2==0 && d3==0) begin
                end
                else if (d0 == 0) begin
                    d0 <= 9;
                    if (d1 == 0) begin
                        d1 <= 9;
                        if (d2 == 0) begin
                            d2 <= 9;
                            d3 <= d3 - 1;
                        end else d2 <= d2 - 1;
                    end else d1 <= d1 - 1;
                end else d0 <= d0 - 1;
            end
        end
    end

    // Displaying the time
    reg [1:0] sel;
    reg [3:0] current_digit;
    reg [16:0] display_clk;

    always @(posedge clk) begin
        if (reset_clean) begin
            sel <= 0;
            display_clk <= 0;
        end else begin
            if (display_clk == 99999) begin
                display_clk <= 0;
                sel <= sel + 1;
            end else
                display_clk <= display_clk + 1;
        end
    end

    always @(*) begin
        case (sel)
            2'b00: begin an = 4'b1110; current_digit = d0; end
            2'b01: begin an = 4'b1101; current_digit = d1; end
            2'b10: begin an = 4'b1011; current_digit = d2; end
            2'b11: begin an = 4'b0111; current_digit = d3; end
            default: begin an = 4'b1111; current_digit = 4'b0; end
        endcase
    end

    hexto7segment h(.x(current_digit), .r(sseg));
    assign dp = (sel == 2'b10) ? 1'b0 : 1'b1;

endmodule