`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/05/2026 08:54:00 PM
// Design Name: 
// Module Name: clk_div
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


module clk_div(
    input clk,
    input reset,
    output reg clk_out
    );
    reg[19:0] COUNT;
        
    always @(posedge clk)
    begin
        if (reset) begin
            COUNT <= 0;
            clk_out <= 0;
        end
        else if (COUNT == 999999) begin
            COUNT <= 0;
            clk_out <= 1;
        end
        else begin
            COUNT <= COUNT +1;
            clk_out <= 0;
        end
    end
endmodule
