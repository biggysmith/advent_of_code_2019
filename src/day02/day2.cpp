#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<int> parse_input(const std::string& file){
    std::vector<int> input;

    std::ifstream file_stream(file);

    std::string line;
    std::getline(file_stream, line);

    std::stringstream ss(line);
    std::string number_str;
    while(std::getline(ss, number_str, ',')){
        input.push_back(std::stoi(number_str));
    }

    return input;
}

void main()
{
    auto input = parse_input("../src/day02/day2_input.txt");

    auto run = [&](int noun,int verb){
        auto prog = input;

        prog[1] = noun;
        prog[2] = verb;

        int i = 0;
        while (prog[i] != 99) {
            if (prog[i] == 1) {
                prog[prog[i+3]] = prog[prog[i+1]] + prog[prog[i+2]];
            }else if (prog[i] == 2) {
                prog[prog[i+3]] = prog[prog[i+1]] * prog[prog[i+2]];
            }else {
                return -1;
            }
            i += 4;
        }

        return prog[0];
    };

    std::cout << "part1: " << run(12,2) << std::endl;

    for(int noun=0; noun<100; ++noun){
        for(int verb=0; verb<100; ++verb){
            if(run(noun,verb) == 19690720){
                std::cout << "part2: " << 100 << " * " << noun << " + " << verb << " = " << 100*noun+verb << std::endl;
                return;
            }
        }
    }
}