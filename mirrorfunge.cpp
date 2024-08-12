#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <random>
#include <sstream>

#define EMBED  0 // Enable this if you are hardcoding directly into this c++ file
#define FILEIO 0 // Enable this if you want to take IO from files

#if FILEIO
std::ifstream INPUT_STREAM("filein.txt");
std::ofstream OUTPUT_STREAM("fileout.txt");
#else
std::istream &INPUT_STREAM = std::cin;
std::ostream &OUTPUT_STREAM = std::cout;
#endif

#if EMBED
std::string hardcoded = 1+R"PROGRAM(
Write your program here (if EMBED is 1)
)PROGRAM";
#endif

std::string VERSION_STRING = 1+R"VERSION(
mirrorfunge-2.1.1
mirrorfunge.cpp 1.0
)VERSION";

const int SENTINEL = -1;

enum Direction : uint8_t {
    RIGHT = 0,
    UP    = 1,
    LEFT  = 2,
    DOWN  = 3,
    SUPER = 4,
};

using stackval_t = long long;

std::random_device rd;
std::mt19937 gen(rd());

stackval_t mtrand(stackval_t a, stackval_t b) {
    if (a > b) {
        std::swap(a, b);
    }
    return (std::uniform_int_distribution<stackval_t>(a, b))(gen);
}

std::vector<std::string> src;
int height, width;
int x, y;
Direction direction;
int step;
bool stringmode;
std::vector<stackval_t> memstack;

Direction mapping_45[4]         = {UP, RIGHT, DOWN, LEFT},
          mapping_135[4]        = {DOWN, LEFT, UP, RIGHT},
          mapping_arrowup[4]    = {UP, DOWN, UP, SUPER},
          mapping_arrowdown[4]  = {DOWN, SUPER, DOWN, UP},
          mapping_arrowleft[4]  = {SUPER, LEFT, RIGHT, LEFT},
          mapping_arrowright[4] = {LEFT, RIGHT, SUPER, RIGHT};

char getInstruction() {
    return (width <= x) ? ' ' : src[y][x];
}

void verticalComp() {
    // For ^ and v
    direction = (memstack.back() == 0) ? RIGHT : LEFT;
    memstack.pop_back();
}

void horizontalComp() {
    // For < and >
    direction = (memstack.back() == 0) ? DOWN : UP;
    memstack.pop_back();
}

void stepIP() {
    switch (direction) {
        case RIGHT:
            x += step;
            break;
        case UP:
            y -= step;
            break;
        case LEFT:
            x -= step;
            break;
        case DOWN:
            y += step;
            break;
    }
    x = (x%width + width)%width;
    y = (y%height + height)%height;
}

class InputWrapper {
public:
    InputWrapper(std::istream &stream): stream(stream), inputbuf() {};
    signed char peekchar() {
        if (inputbuf.empty()) {
            read();
        }
        if (inputbuf.empty()) {
            return SENTINEL;
        }
        return inputbuf.front();
    }
    signed char getchar() {
        if (inputbuf.empty()) {
            read();
        }
        if (inputbuf.empty()) {
            return -1;
        }
        char ret = inputbuf.front();
        inputbuf.pop();
        return ret;
    }
    stackval_t getnum() {
        std::string strnum;
        int c;
        while (c = peekchar(), c != SENTINEL && isspace(c)) {
            inputbuf.pop();
        }
        while (c = peekchar(), c != SENTINEL && (c == '-' || c== '+')) {
            strnum.push_back(inputbuf.front());
            inputbuf.pop();
        }
        while (c = peekchar(), c != SENTINEL && '0' <= c && c <= '9') {
            strnum.push_back(inputbuf.front());
            inputbuf.pop();
        }
        return std::stoll(strnum);
    }
private:
    std::istream &stream;
    std::queue<unsigned char> inputbuf;
    void read() {
        std::string line;
        if (getline(stream, line)) {
            line.push_back('\n');
        }
        for (const unsigned char &c : line) {
            inputbuf.push(c);
        }
    }
};

int main(int argc, char *argv[]) {
    step = 1;
#if EMBED
    std::stringstream source(hardcoded);
#else
    std::string fpath;
    if (argc > 1) {
        fpath = argv[1];
    } else {
        std::cerr << "Please provide a file\n";
        return 0;
    }
    if (fpath == "--version") {
        std::cout << VERSION_STRING;
        return 0;
    }
    std::ifstream source(fpath);
    if (!source.is_open()) {
        std::cerr << "That file does not exist\n";
        return 0;
    }
#endif
    InputWrapper in(INPUT_STREAM);
    std::ostream &out = OUTPUT_STREAM;
    std::string line;
    while (getline(source, line)) {
        src.push_back(line);
        width = std::max(width, (int)line.size());
    }
    height = src.size();
#if !EMBED
    source.close();
#endif
    if (height <= 0 || width <= 0) return 0;
    while (1) {
        char instruction = getInstruction();
        if (stringmode) {
            if (instruction == '"') {
                stringmode = false;
            } else {
                memstack.push_back(instruction);
            }
        } else {
            switch (instruction) { // 0-depth instructions
                case '/':
                    direction = mapping_45[direction];
                    break;
                case '\\':
                    direction = mapping_135[direction];
                    break;
                case 'x':
                    direction = (Direction)((direction+2)%4);
                    break;
                case '^':
                    direction = mapping_arrowup[direction];
                    if (direction == SUPER) verticalComp();
                    break;
                case 'v':
                    direction = mapping_arrowdown[direction];
                    if (direction == SUPER) verticalComp();
                    break;
                case '<':
                    direction = mapping_arrowleft[direction];
                    if (direction == SUPER) horizontalComp();
                    break;
                case '>':
                    direction = mapping_arrowright[direction];
                    if (direction == SUPER) horizontalComp();
                    break;
                case '#':
                    stepIP();
                    break;
                case '@':
                    goto end; // end
                    break;
                case '&':
                    if (in.peekchar() == SENTINEL) {
                        direction = (Direction)((direction+2)%4);
                    } else {
                        memstack.push_back(in.getnum());
                    }
                    break;
                case '~':
                    memstack.push_back(in.getchar());
                    break;
                case '"':
                    stringmode = true;
                    break;
                case '=':
                    std::stringstream ss;
                    ss << "[";
                    for (int i = 0; i < memstack.size(); ++i) {
                        ss << memstack[i];
                        if (i < memstack.size()-1) {
                            ss << ", ";
                        }
                    }
                    ss << "]";
                    out << ss.str() << "\n";
                    break;
            }
            if ('0' <= instruction && instruction <= '9') {
                memstack.push_back(instruction-'0');
            } else if ('a' <= instruction && instruction <= 'f') {
                memstack.push_back(instruction-'a'+10);
            }
            if (memstack.size() >= 1) {
                switch (instruction) { // 1-depth instructions
                    case ',':
                        out << (char)memstack.back();
                        memstack.pop_back();
                        break;
                    case '.':
                        out << memstack.back();
                        memstack.pop_back();
                        break;
                    case ')':
                        ++memstack.back();
                        break;
                    case '(':
                        --memstack.back();
                        break;
                    case ':':
                        memstack.push_back(memstack.back());
                        break;
                    case '!':
                        memstack.back() = (memstack.back() == 0) ? 1 : 0;
                        break;
                    case '$':
                        memstack.pop_back();
                        break;
                }
            }
            if (memstack.size() >= 2) {
                switch (instruction) { // 2-depth instructions
                    case '+':
                        {
                        stackval_t tmp = memstack.back();
                        memstack.pop_back();
                        memstack.back() += tmp;
                        }
                        break;
                    case '-':
                        {
                        stackval_t tmp = memstack.back();
                        memstack.pop_back();
                        memstack.back() -= tmp;
                        }
                        break;
                    case '*':
                        {
                        stackval_t tmp = memstack.back();
                        memstack.pop_back();
                        memstack.back() *= tmp;
                        }
                        break;
                    case '|':
                        {
                        stackval_t tmp = memstack.back();
                        memstack.pop_back();
                        memstack.back() = (tmp == 0) ? 0 : memstack.back()/tmp;
                        }
                        break;
                    case ';':
                        {
                        stackval_t a = memstack.back();memstack.pop_back();
                        stackval_t b = memstack.back();memstack.pop_back();
                        memstack.push_back(a);
                        memstack.push_back(b);
                        }
                        break;
                    case '`':
                        {
                        stackval_t a = memstack.back();memstack.pop_back();
                        stackval_t b = memstack.back();memstack.pop_back();
                        memstack.push_back(b > a);
                        }
                        break;
                    case '?':
                        {
                        stackval_t a = memstack.back();memstack.pop_back();
                        stackval_t b = memstack.back();memstack.pop_back();
                        memstack.push_back(mtrand(a, b));
                        }
                        break;
                }
            }
            switch (instruction) { // variable-depth instructions
                case 'r':
                    {
                    stackval_t backpos = memstack.back();
                    memstack.pop_back();
                    if (backpos > memstack.size() || backpos <= 0) break;
                    memstack.push_back(memstack.end()[-backpos-1]);
                    memstack.erase(memstack.end()-backpos-2);
                    }
                    break;
                case 's':
                    {
                    stackval_t backpos = memstack.back();
                    memstack.pop_back();
                    if (backpos > memstack.size() || backpos <= 0) break;
                    std::swap(memstack.end()[-1], memstack.end()[-backpos-1]);
                    }
                    break;
                case 'g':
                    {
                    stackval_t backpos = memstack.back();
                    memstack.pop_back();
                    if (backpos > memstack.size() || backpos <= 0) break;
                    memstack.push_back(memstack.end()[-backpos-1]);
                    }
                    break;
            }
        }
        stepIP();
    }
end:
    return 0;
}
