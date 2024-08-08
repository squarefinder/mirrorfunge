Sections:
 - [Control flow](#control-flow)
 - [IO](#io)
 - [String mode](#string-mode)
 - [Literals](#literals)
 - [Operations](#operations)
 - [Stack management](#stack-management)

# Control flow
In this section, U, D, L, R represent up, down, left and right
respectively. The following is an explanation of the notations used:
```
Notation        Explanation
U => R          If the IP is facing up when it executes this instruction,
                change to face right
U <=> R         Shorthand for U => R, R => U
```
```
Instruction     Action
/               U <=> R, D <=> L
\               U <=> L, D <=> R
x               U <=> D, L <=> R
^               R => U, L => U, U => D, D => 
v               R => D, L => D, D => U, U =>
<               U => L, D => L, L => R, R =>
>               U => R, D => R, R => L, L =>
#               Skip over the next instruction (move the IP two spaces
                this tick instead of one space)
@               End the program
```

# IO
```
Instruction     Action
&               Take a continuous series of signs then a continuous series
                of digits as a number from input, discarding any preceding
                whitespace. Push this number to the stack. If there is no
                more input, make the IP face the opposite direction.
~               Take a single character from input. Push the ascii value
                of this character to the stack. If there is no more input,
                push -1.
=               Debug statement; Dump the entire stack to output
,               Pop x from the stack, print x as a character to output
.               Pop x from the stack, print x as a number to output
```

# String mode
When the IP encounters the `"` character, it will toggle the string mode
state. In string mode, all characters other than `"` and `\n` are taken
literally and their ascii value pushed to the stack.

# Literals
```
Instruction     Action
0               Push 0 to the stack
1               Push 1 to the stack
2               Push 2 to the stack
3               Push 3 to the stack
4               Push 4 to the stack
5               Push 5 to the stack
6               Push 6 to the stack
7               Push 7 to the stack
8               Push 8 to the stack
9               Push 9 to the stack
a               Push 10 to the stack
b               Push 11 to the stack
c               Push 12 to the stack
d               Push 13 to the stack
e               Push 14 to the stack
f               Push 15 to the stack
```

# Operations
```
Instruction     Action
)               Pop x, push x+1
(               Pop x, push x-1
!               Pop x, if x is 0 push 1, otherwise push 0
+               Pop x then y, push y+x
-               Pop x then y, push y-x
*               Pop x then y, push y*x
|               Pop x then y, if x is not 0 push x/y, otherwise push 0
`               Pop x then y, if y > x push 1, otherwise push 0
?               Pop x then y, push a random number between x and y
                inclusive
```

# Stack management
```
Instruction     Action
:               Pop x, push x, push x
;               Pop x then y, push x then y
$               Pop x
r               Pop x, then move the xth element from the top of the stack
                up to the top of the stack
s               Pop x, then swap the xth element from the top of the stack
                with the element at the top of the stack
g               Pop x, then copy the xth element from the top of the stack
                and push it to the top of the stack
```

*Clarifications on `r`(etrieve), `s`(wap) and `g`(et) instructions*

The stack is 0-indexed from the top of the stack, and the index is taken
after the initial value is popped. For example:

If the stack is initially as follows (push and pop from the right):
```
3 5 7 9 11 3
```
Then the `r` instruction would be executed as follows:
```
3 5 7 9 11 3
3 5 7 9 11              Pop 3
3 7 9 11                Remove the element that is index 3 from the top
3 7 9 11 5              Push that element
```
