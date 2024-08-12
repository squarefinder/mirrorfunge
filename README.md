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

The main memory of the language is the element stack (ES). Most
instructions operate on elements at the top of the stack, but a few can
retrieve values from lower positions. From here, a ‘stack’ refers to the
ES.

### Custom Functions

Each of the uppercase letters can be bound to a custom function. (Let us
call any uppercase letter a ‘name’). An unbound name acts as a no-op.

From here on, FS refers to the ‘function stack’, a stack with each element
referring to a nested function call, with a stack frame. ‘stack’ still
refers to an ES.

Each stack frame contains an ES, and a reference to the position (and
possibly direction) of the IP when the function was called. The program
begins with a single stack frame on the FS, and ends when all stack frames
have been popped.

When a custom function is called, a set number of elements are popped off
the ES of the stack frame currently at the top of the FS, a new stack
frame is pushed to the FS, the popped elements are added to the ES of this
new stack frame in their original order, the position and direction of the
IP are added to the new stack frame, and the IP is moved to the starting
position of the function. The function runs until it reaches the `@`
instruction, at which point it returns, pushing the entire ES to the ES
of the previous stack frame, returning the IP to its original position
and direction, and popping the stack frame off the FS.

In general, commands that bind a name to a function will take four numbers
as input, `x`, `y`, `n` and `k`. `x` and `y` are coordinates for the
starting position of the function. `k` is the number of elements moved
from the previous ES to the new ES, `-1` means to take the entire ES.
`n` is the name.
`n` can be any of the following mappings (inclusive):
- 0 - 25    (0-indexed where 0 represents `A` and 25 represents `Z`)
- 65 - 90   (uppercase alphabet)
- 97 - 122  (lowercase alphabet)
To be clear, `n = 0`, `n = 65` and `n = 97` will all behave *exactly* the
same and bind the function to the name `A`.

Plan
- Imports
- Directional functions
    - The direction of the IP is passed into the function when entering
    - The direction of the IP is passed out of the function when exiting
- Inert functions
    - The function does not change the direction of the IP
    - The IP returns to its original state once the function has returned
    - The IP always starts the function facing right

#### Custom Functions Implementation Progress
- mirrorfunge.cpp
    - Function Stack implemented
- pyflect.py
    - Nothing

## Instructions

See the [instruction guide](instruction_guide.md).
