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

void main()
{
    auto input = parse_input("../src/day13/day13_input.txt");

    int width = 35;
    int height = 23;
    std::vector<int> screen(width*height,0);

    auto print_screen = [&]{
        for(int y=0; y<height; ++y){
            for(int x=0; x<width; ++x){
                std::cout << screen[y*width+x];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::cout.flush();
    };

    auto find_tile_pos = [&](int t){
        auto it = std::find(screen.begin(), screen.end(), t);
        int pos = (int)std::distance(screen.begin(), it);
        return std::make_pair(pos % width, pos / width);
    };

    int score = 0;
    
    auto run = [&](bool print,bool free_to_play){
        auto& prog = input;

        if(free_to_play){
            prog[0] = 2;
        }

        int x = 0;
        int y = 0;
        int cmd_num = 0;

        long long i = 0;
        long long relative_base = 0; 

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
                auto paddle_pos = find_tile_pos(3);
                auto ball_pos = find_tile_pos(4);
                if(paddle_pos.first > ball_pos.first){
                    param(1) = -1;
                }else if(paddle_pos.first == ball_pos.first){
                    param(1) = 0;
                }else{
                    param(1) = 1;
                }
                if(print){
                    gotoxy(0,0);
                    print_screen();
                }
                i += 2;
            }else if (op_code == 4) {
                if(cmd_num == 0){
                    x = (int)param(1);
                }else if(cmd_num == 1){
                    y = (int)param(1);
                }else if(cmd_num == 2){
                    if(x==-1 &&  y==0){
                        score = (int)param(1);
                        if(std::count_if(screen.begin(), screen.end(), [](auto& tile){ return tile == 2; }) <= 0){
                            return;
                        }
                    }else{
                        screen[y*width+x] = (int)param(1);
                    }
                }
                cmd_num = (cmd_num + 1) % 3;
                i += 2;

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

    };


    // part 1
    {
        run(false, false);
        std::cout << "part1: " << std::count_if(screen.begin(), screen.end(), [](auto& tile) { return tile == 2; }) << std::endl;
    }

    // part 2
    {
        run(false, true);
        std::cout << "part2: " << score << std::endl;
    }
}
