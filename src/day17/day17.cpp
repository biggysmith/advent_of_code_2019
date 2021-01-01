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

int run(state_t& state,const std::string& input,int& n){
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
            param(1) = input[n++];
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

void main()
{
    auto input = parse_input("../src/day17/day17_input.txt");

    state_t state = { input, 0, 0 };

    int width = 0;
    int height = 0;
    std::vector<char> screen;

    int x = 0;
    int y = 0;
    int n = 0;

    int cam = 0;
    do{

        cam = run(state,"",n);
        if(cam != '\n'){
            screen.push_back(cam);
        }else if(width == 0){
            width = (int)screen.size();
        }

    }while(cam >= 0);

    height = (int)screen.size() / width;

    auto val = [&](int x,int y) -> auto&{
        return screen[y*width+x];
    };

    int sum = 0;
    for(int y=1; y<height-1; ++y){
        for(int x=1; x<width-1; ++x){

            if(val(x,y  ) == '#' && 
               val(x,y-1) == '#' && 
               val(x,y+1) == '#' &&
               val(x-1,y) == '#' &&
               val(x+1,y) == '#')
            {
                val(x,y) = 'O';
                sum += x*y;
            }

        }
    }

    std::cout << "part1: " << sum << std::endl;

    // paths figured out by hand!
    std::string main_routine = "A,B,A,C,B,A,C,B,A,C\n";
    std::string A = "L,12,L,12,L,6,L,6\n";
    std::string B = "R,8,R,4,L,12\n";
    std::string C = "L,12,L,6,R,12,R,8\n";
    std::string video = "n\n";

    std::string main_str = main_routine + A + B + C + video;

    n = 0;
    state = { input, 0, 0 };
    state.prog[0] = 2;

    int cmd = 0;
    do{
        cmd = run(state,main_str,n);
        if(cmd > 127){
            std::cout << "part2: " << cmd << std::endl;
        }else{
            std::cout << (char)cmd;
        }
    }while(cmd >= 0);

}
