# Mirrorfunge

A 2D stack-based language based on Befunge.

## Concepts

### The Instruction Pointer

The instruction pointer (abbreviated IP) is a coordinate that points to
the next instruction to execute in program space. Every 'tick', it moves
one space in the direction it is facing. The IP's direction can be
changed by several instructions, which act as mirrors.

### Program Space

The program space is a two dimensional grid with height equal to the
number of lines in the program, and width equal to the maximum number
of characters in a line. When the IP reaches a border, it is wrapped
around to the other side i.e. the left and right borders are directly
connected, and the top and bottom borders are directly connected.

### The Stack

The main memory of the language is the stack. Most instructions operate
on elements at the top of the stack, but a few can retrieve values from
lower positions.

### Instructions

See the [instruction guide](instruction_guide.md).
