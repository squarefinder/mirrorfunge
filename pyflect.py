#!/usr/bin/python3

# A python interpreter of mirrorfunge

VERSION_STRING = """
mirrorfunge-2.1.1
pyflect 2.1.1
"""[1:]

import sys, random, collections

class Stack:
    def __init__(self):
        self.stack = list()

    def __str__(self):
        return str(self.stack)

    def __len__(self):
        return len(self.stack)

    def __bool__(self):
        return bool(self.stack)

    def top(self):
        if not self:
            return 0
        return self.stack[-1]

    def push(self, value):
        self.stack.append(value)

    def pop(self):
        if not self:
            return 0
        return self.stack.pop()

class Queue:
    def __init__(self):
        self.deque = collections.deque()

    def __str__(self):
        return str(self.deque)

    def __len__(self):
        return len(self.deque)

    def __bool__(self):
        return bool(self.deque)

    def front(self):
        return self.deque[0]

    def push(self, value):
        self.deque.append(value)

    def pop(self):
        return self.deque.popleft()

class InputStream:
    def __init__(self, stream):
        self.stream = stream
        self.queue = Queue()

    def __read(self):
        for i in self.stream.readline():
            self.queue.push(i)

    def peekchar(self):
        if not self.queue:
            self.__read()
        if not self.queue:
            return -1
        return self.queue.front()

    def getchar(self):
        if not self.queue:
            self.__read()
        if not self.queue:
            return -1
        return self.queue.pop()

    def getnum(self):
        x = []
        while (c := self.peekchar(), c != -1 and c.isspace())[-1]:
            self.queue.pop()
        while (c := self.peekchar(), c != -1 and c.isnumeric())[-1]:
            x.append(self.queue.pop())
        return int("".join(x))

src = []
width = 0
height = 0
stack = Stack()

def outofbounds(x,y):
    return x < 0 or y < 0 or x >= width or y >= height

def wrap(x,y):
    return (x%width, y%height)

def getchar(x,y):
    if len(src[y]) <= x:
        return " "
    else:
        return src[y][x]

def getnewdir1(direction): # /
    if direction == 0:
        return 1
    elif direction == 1:
        return 0
    elif direction == 2:
        return 3
    else:
        return 2

def getnewdir2(direction): # \
    if direction == 0:
        return 3
    elif direction == 1:
        return 2
    elif direction == 2:
        return 1
    else:
        return 0

def arrow_up(direction): # ^ acts as both a merger and a comparator
    if direction == 0:
        return 1
    elif direction == 1:
        return 3
    elif direction == 2:
        return 1
    else:
        return 0 if stack.pop() == 0 else 2

def arrow_down(direction): # v acts as both a merger and a comparator
    if direction == 0:
        return 3
    elif direction == 1:
        return 0 if stack.pop() == 0 else 2
    elif direction == 2:
        return 3
    else:
        return 1

def arrow_left(direction): # < acts as both a merger and a comparator
    if direction == 0:
        return 3 if stack.pop() == 0 else 1
    elif direction == 1:
        return 2
    elif direction == 2:
        return 0
    else:
        return 2

def arrow_right(direction): # > acts as both a merger and a comparator
    if direction == 0:
        return 2
    elif direction == 1:
        return 0
    elif direction == 2:
        return 3 if stack.pop() == 0 else 1
    else:
        return 0

def newpos(direction, x, y, stepsize=1):
    if direction == 0:
        return (x+stepsize, y)
    elif direction == 1:
        return (x, y-stepsize)
    elif direction == 2:
        return (x-stepsize, y)
    else:
        return (x, y+stepsize)

def ishexletter(char):
    return ord(char) >= ord("a") and ord(char) <= ord("f")

if __name__ == "__main__":
    cin = InputStream(sys.stdin)
    if len(sys.argv) > 1:
        fpath = sys.argv[1]
    else:
        print("Please provide a file", file=sys.stderr)
        exit(0)
    if fpath == "--version":
        print(VERSION_STRING, end='')
        exit(0)
    try:
        with open(fpath) as source:
            line = source.readline()
            while line != "":
                src.append(line.rstrip())
                height += 1
                currwidth = len(line.rstrip())
                if currwidth > width:
                    width = currwidth
                line = source.readline()
    except FileNotFoundError:
        print("That file does not exist", file=sys.stderr)
        exit(0)
    if len(src) == 0:
        exit()
    if len(src[0]) == 0:
        exit()
    x = 0         #   1
    y = 0         # 2   0
    direction = 0 #   3
    stringmode = False
    while True:
        if outofbounds(x,y):
            x, y = wrap(x,y)
        char = getchar(x,y)
        step = 1
        if stringmode:
            if char == "\"":
                stringmode = False
            else:
                stack.push(ord(char))
        else:
            if char == "/":
                direction = getnewdir1(direction)
            if char == "\\":
                direction = getnewdir2(direction)
            if char == "x":
                direction = (direction+2)%4
            if char == "^":
                # this is a merger / comparator
                direction = arrow_up(direction)
            if char == "v":
                # this is a merger / comparator
                direction = arrow_down(direction)
            if char == "<":
                # this is a merger / comparator
                direction = arrow_left(direction)
            if char == ">":
                # this is a merger / comparator
                direction = arrow_right(direction)
            if char == "#":
                # jump
                x,y = newpos(direction, x, y, step)
            if char == "@":
                # end
                break
            if char == "&":
                sys.stdout.flush()
                if cin.peekchar() == -1:
                    direction = (direction+2)%4
                else:
                    a = cin.getnum()
                    stack.push(a)
            if char == "~":
                sys.stdout.flush()
                inputchar = cin.getchar()
                if inputchar == -1:
                    stack.push(-1)
                else:
                    stack.push(ord(inputchar))
            if char == "\"":
                stringmode = True
            if char.isnumeric():
                stack.push(int(char))
            if ishexletter(char):
                stack.push(ord(char)-ord("a")+10)
            if char == "=":
                print(stack)
            if len(stack) >= 1:
                if char == ",":
                    sys.stdout.write(chr(stack.pop()))
                if char == ".":
                    sys.stdout.write(str(stack.pop()))
                if char == ")":
                    stack.push(stack.pop()+1)
                if char == "(":
                    stack.push(stack.pop()-1)
                if char == ":":
                    stack.push(stack.top())
                if char == "!":
                    a = stack.pop()
                    if a == 0:
                        stack.push(1)
                    else:
                        stack.push(0)
                if char == "$":
                    stack.pop()
            if len(stack) >= 2:
                if char == "+":
                    stack.push(stack.pop()+stack.pop())
                if char == "-":
                    stack.push(-stack.pop()+stack.pop())
                if char == "*":
                    stack.push(stack.pop()*stack.pop())
                if char == "|":
                    a = stack.pop()
                    b = stack.pop()
                    if a == 0:
                        stack.push(0)
                    else:
                        stack.push(b//a)
                if char == ";":
                    a = stack.pop()
                    b = stack.pop()
                    stack.push(a)
                    stack.push(b)
                if char == "`":
                    a = stack.pop()
                    b = stack.pop()
                    if b > a:
                        stack.push(1)
                    else:
                        stack.push(0)
                if char == "?":
                    a = stack.pop()
                    b = stack.pop()
                    stack.push(random.randint(a, b) if a <= b else random.randint(b, a))
            if char == "r": # r for retrieval (long retrieval)
                a = stack.pop()
                if a > len(stack) or a <= 0:
                    pass
                else:
                    templist = []
                    for i in range(a):
                        templist.append(stack.pop())
                    b = stack.pop()
                    templist.reverse()
                    for i in templist:
                        stack.push(i)
                    stack.push(b)
            if char == "s": # s for swap (long swap)
                a = stack.pop()
                if a > len(stack) or a <= 0:
                    pass
                else:
                    templist = []
                    c = stack.pop()
                    for i in range(a-1):
                        templist.append(stack.pop())
                    b = stack.pop()
                    templist.reverse()
                    stack.push(c)
                    for i in templist:
                        stack.push(i)
                    stack.push(b)
            if char == "g": # g for get (long duplicate)
                a = stack.pop()
                if a > len(stack) or a <= 0:
                    pass
                else:
                    stack.push(stack.stack[-a-1])
        x,y = newpos(direction, x, y, step)
