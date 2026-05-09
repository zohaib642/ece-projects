# ece-projects

**Street Fighter (MSPM0G3507 Embedded Game)**
A 1v1 fighting game running on a custom PCB designed in KiCad and hand-soldered. Built around the MSPM0G3507 microcontroller with custom hardware drivers and a 30 Hz game engine running on a TimerG12 ISR. Player movement is driven by a slidepot ADC channel, a 2-axis analog joystick handles jumping, and four external buttons map to punch, kick, block, and pause. Sampled audio plays through a 5-bit DAC over an 11 kHz SysTick interrupt, providing background music and event-driven sound effects (hit, block, KO). Features include 16-bit color sprite rendering on an ST7735 LCD with edge-only erase to minimize flicker, an enemy AI with attack/block/movement states, real-time HP and score tracking, and bilingual (English/Spanish) UI selectable at startup.

**Programmable Stopwatch/Timer (Verilog on Basys3 FPGA)**
A four-mode digital stopwatch and countdown timer implemented in Verilog using RTL-design methodology and deployed to a Digilent Basys3 FPGA. Designed via HLSM formalism with a custom datapath and controller FSM. Supports counting up from 00.00, counting down from 99.99, and externally loaded variants where the initial seconds value is set through eight onboard switches. Time is shown across four seven-segment displays at 10 ms resolution, with two pushbuttons for start/stop and reset and two mode-select switches. Built and verified in Xilinx Vivado.

**Buck Converter (PWM Control)**
Design and analysis of a DC-DC buck converter circuit using PWM control. Explores the relationship between switching frequency (50-100 kHz), duty cycle, and output voltage regulation. Includes current sensing measurements and characterization of the linear relationship between op-amp output and load voltage.

**Stone Game (LC-3 Assembly)**
A two-player Nim-style strategy game implemented in LC-3 assembly language. Players alternate removing stones from three rows (A, B, C), with the player who takes the last stone winning. Features input validation, game state management, and a complete turn-based game loop.
