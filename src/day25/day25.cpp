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
#include <queue>

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



int run(state_t& state){
    auto& prog = state.prog;

    long long& i = state.i;
    long long& relative_base = state.relative_base; 

    auto code = [&](long long p) -> auto&{
        if(p >= (long long )prog.size()){
            prog.insert(prog.end(), p+1-prog.size(), 0);
        }
        return prog[p];
    };

    bool processed_str = true;
    std::string input;
    int n = 0;
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
            if(input.empty()){
                std::getline(std::cin, input);
                input += "\n";
            }
            param(1) = input.front();
            input = input.substr(1);
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
    auto input = parse_input("../src/day25/day25_input.txt");

    state_t state = { input, 0, 0 };

    std::string main_str;
    main_str += "north\n";

    int n = 0;
    int cmd = 0;
    do {
        cmd = run(state);
        std::cout << (char)cmd;
    } while (cmd >= 0);

    /*
        played manually
        - dehydrated water
        - candy cane
        - dark matter
        - bowl of rice
        password: 10504192
    */
}
