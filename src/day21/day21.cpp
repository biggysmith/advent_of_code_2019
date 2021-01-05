#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <chrono>

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
    auto input = parse_input("../src/day21/day21_input.txt");

    {
        state_t state = { input, 0, 0 };

        std::string main_str;
        main_str += "OR A T\n";
        main_str += "AND B T\n";
        main_str += "AND C T\n";
        main_str += "NOT T J\n";
        main_str += "AND D J\n";
        main_str += "WALK\n";

        int n = 0;
        int cmd = 0;
        do {
            cmd = run(state, main_str, n);
            if (cmd > 127) {
                std::cout << "part1: " << cmd << std::endl;
            } else {
                std::cout << (char)cmd;
            }
        } while (cmd >= 0);
    }


    {
        state_t state = { input, 0, 0 };
        
        std::string main_str;
        main_str += "OR A T\n";
        main_str += "AND B T\n";
        main_str += "AND C T\n";
        main_str += "NOT T J\n";
        main_str += "AND D J\n";
        main_str += "OR E T\n";
        main_str += "OR H T\n";
        main_str += "AND T J\n";
        main_str += "RUN\n";

        int n = 0;
        int cmd = 0;
        do {
            cmd = run(state, main_str, n);
            if (cmd > 127) {
                std::cout << "part2: " << cmd << std::endl;
            } else {
                std::cout << (char)cmd;
            }
        } while (cmd >= 0);
    }

}
