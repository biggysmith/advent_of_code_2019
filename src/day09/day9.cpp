#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

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

    input.insert(input.end(),1<<16,0);

    return input;
}

void main()
{
    auto input = parse_input("../src/day09/day9_input.txt");

    auto run = [&](){
        auto prog = input;

        long long i = 0;
        long long relative_base = 0;
        while (prog[i] != 99) {
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(5) << prog[i];
            std::string str = ss.str();
            int op_code = std::stoi(str.substr(3,5));

            auto mode  = [&](long long p){ return str[3-p]; };
            auto val   = [&](long long p){ return prog[i+p]; };
            auto addr  = [&](long long p){ return prog[val(p)]; };
            auto rel   = [&](long long p){ return prog[relative_base+val(p)]; };
            auto param = [&](long long p){ return mode(p)=='1' ? val(p) : (mode(p)=='2' ? rel(p) : addr(p)); };
            auto write = [&](long long v){ (mode(3)=='2' ? prog[relative_base+prog[i+3]] : prog[prog[i+3]]) = v; };

            if (op_code == 1) {
                write(param(1) + param(2));
                i += 4;
            }else if (op_code == 2) {
                write(param(1) * param(2));
                i += 4;
            }else if (op_code == 3) {
                std::cout << "Enter input: ";
                std::cin >> (mode(1)=='2' ? prog[relative_base+prog[i+1]] : prog[prog[i+1]]);
                std::cout << std::endl;
                i += 2;
            }else if (op_code == 4) {
                std::cout << param(1) << std::endl;
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
                write(param(1) < param(2));
                i += 4;
            }else if (op_code == 8) {
                write(param(1) == param(2));
                i += 4;
            }else if (op_code == 9) {
                relative_base += param(1);
                i += 2;
            }else {
                std::cout << "ErRoR!" << std::endl;
                return;
            }
        }
    };

    run();
}

// prog[prog[i+3]] = (str[2]=='1' ? prog[i+1] : prog[prog[i+1]]) + (str[1]=='1' ? prog[i+2] : prog[prog[i+2]]);