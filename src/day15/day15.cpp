#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <emmintrin.h>
#include <smmintrin.h>
#include <chrono>
#include <conio.h>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>

std::vector<long long> parse_input(const std::string& file){
    std::vector<long long> input;

    std::ifstream file_stream(file);

    std::string line;
    std::getline(file_stream, line);

    std::stringstream ss(line);
    std::string number_str;
    while(std::getline(ss, number_str, ',')){
        input.push_back(std::stoll(number_str));
    }

    return input;
}

void gotoxy(int x, int y) { 
    COORD pos = {(short)x, (short)y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

struct state_t{
    std::vector<long long> prog;
    long long i;
    long long relative_base; 

    std::vector<char> screen;
    int width;
    int height;

    int tx;
    int ty;
    int path_len;
    int max_steps;
};

void print_screen(const std::vector<char>& screen,int width,int height){
    for(int y=0; y<height; ++y){
        for(int x=0; x<width; ++x){
            std::cout << screen[y*width+x];
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout.flush();
};

int run(state_t& state,int dir){
    auto& prog = state.prog;

    long long& i = state.i;
    long long& relative_base = state.relative_base; 

    auto code = [&](long long p) -> auto&{
        if(p >= (long long )prog.size()){
            prog.insert(prog.end(), p+1-prog.size(), 0);
        }
        return prog[p];
    };

    int out_count = 0;
    while (code(i) != 99) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << code(i);
        std::string str = ss.str();
        int op_code = std::stoi(str.substr(3,5));

        auto mode  = [&](long long p){ return str[3-p]; };
        auto param = [&](long long p) -> auto&{ 
            if(mode(p)=='0'){
                return code(code(i+p));
            }else if(mode(p)=='1'){
                return code(i+p);
            }else /*if(mode(p)=='2')*/{
                return code(relative_base+code(i+p));
            }
        };

        if (op_code == 1) {
            param(3) = param(1) + param(2);
            i += 4;
        }else if (op_code == 2) {
            param(3) = param(1) * param(2); 
            i += 4;
        }else if (op_code == 3) {
            param(1) = dir;
            i += 2;
        }else if (op_code == 4) {
            int output = (int)param(1);
            i += 2;
            return output;
        }else if (op_code == 5) {
            if(param(1) != 0){
                i = param(2);
            }else{
                i += 3;
            }
        }else if (op_code == 6) {
            if(param(1) == 0){
                i = param(2);
            }else{
                i += 3;
            }
        }else if (op_code == 7) {
            param(3) = param(1) < param(2);
            i += 4;
        }else if (op_code == 8) {
            param(3) = param(1) == param(2);
            i += 4;
        }else if (op_code == 9) {
            relative_base += param(1);
            i += 2;
        }else {
            std::cout << "ErRoR!" << std::endl;
        }
    }

    return -1;
};



void solve_maze(state_t& state,int x,int y,int dir)
{
    int idx;
    if (dir == 1) {
         idx = (y-1)*state.width+x;
    }else if (dir == 2) {
        idx = (y+1)*state.width+x;
    }else if (dir == 3) {
        idx = y*state.width+x-1;
    }else if (dir == 4) {
        idx = y*state.width+x+1;
    }

    if (state.screen[idx] == 3 || state.screen[idx] == 1) {
        return;
    }

    auto status = run(state,dir);
    if (status == 0) {
        state.screen[idx] = 1;
        return;
    }

    if (dir == 1) {
        y--;
    }else if (dir == 2) {
        y++;
    }else if (dir == 3) {
        x--;
    }else if (dir == 4) {
        x++;
    }

    state.screen[y*state.width+x] = 3;

    if (status == 2) {
        state.tx = x;
        state.ty = y;
        state.path_len = (int)std::count(state.screen.begin(), state.screen.end(), 3);
    } 

    solve_maze(state, x, y,1);
    solve_maze(state, x, y,2);
    solve_maze(state, x, y,3);
    solve_maze(state, x, y,4);

    // dir didnt work out, clear and reverse dir
    state.screen[y*state.width+x] = 0;

    if (dir == 1) {
        dir = 2;
    }else if (dir == 2) {
        dir = 1;
    }else if (dir == 3) {
        dir = 4;
    }else if (dir == 4) {
        dir = 3;
    }

    status = run(state,dir);
}

void solve_maze(state_t& state,int x,int y)
{
    solve_maze(state, x, y, 1);
    solve_maze(state, x, y, 2);
    solve_maze(state, x, y, 3);
    solve_maze(state, x, y, 4);
}


void solve_fill(state_t& state,int x,int y,int step){
    if(state.screen[y*state.width+x] == 4 || state.screen[y*state.width+x] == 1){
        return;
    }

    state.screen[y*state.width+x] = 4;

    solve_fill(state, x, y-1, step+1);
    solve_fill(state, x, y+1, step+1);
    solve_fill(state, x-1, y, step+1);
    solve_fill(state, x+1, y, step+1);

    state.max_steps = std::max(state.max_steps, step);
}

void main()
{
    state_t state;
    state.prog = parse_input("../src/day15/day15_input.txt");
    state.width = 50;
    state.height = 50;
    state.i = 0;
    state.relative_base = 0;
    state.screen.resize(state.width*state.height, 0); 

    solve_maze(state, state.width/2, state.height/2);
    std::cout << "part1: " << state.path_len << std::endl;

    solve_fill(state, state.tx, state.ty, 0);
    std::cout << "part2: " << state.max_steps << std::endl;
}
