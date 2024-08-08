# Mirrorfunge

A 2D stack-based language based on Befunge.

## Concepts

### The Instruction Pointer

The instruction pointer (abbreviated IP) is a coordinate that points to
the next instruction to execute in program space. Every 'tick', it moves
one space in the direction it is facing. The IP's direction can be
changed by several instructions, which act as mirrors.

### The Stack

The main memory of the language is the stack. Most instructions operate
on elements at the top of the stack, but a few can retrieve values from
lower positions.


