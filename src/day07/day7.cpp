#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

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

struct state_t{
    int i;
    int phase;
    bool phase_set;
    std::vector<int> prog;
};

int main()
{
    const auto input = parse_input("../src/day07/day7_input.txt");

    int result = 0;
    bool finished = false;

    auto run = [&](state_t& state){
        auto& prog = state.prog;
        int& i = state.i;
        while (prog[i] != 99) {
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(5) << prog[i];
            std::string str = ss.str();
            int op_code = std::stoi(str.substr(3,5));

            auto mode  = [&](int p){ return str[3-p] == '1'; };
            auto val   = [&](int p){ return prog[i+p]; };
            auto addr  = [&](int p){ return prog[val(p)]; };
            auto param = [&](int p){ return mode(p) ? val(p) : addr(p); };
            auto write = [&](int v){ return prog[prog[i+3]] = v; };

            if (op_code == 1) {
                write(param(1) + param(2));
                i += 4;
            }else if (op_code == 2) {
                write(param(1) * param(2));
                i += 4;
            }else if (op_code == 3) {
                prog[prog[i+1]] = state.phase_set ? result : state.phase;
                state.phase_set = true;
                i += 2;
            }else if (op_code == 4) {
                result = param(1);
                i += 2;
                return;
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
            }else {
                std::cout << "ErRoR! 0" << std::endl;
                return;
            }
        }

        finished = true;
        return;
    };

    auto amp_run = [&](const std::string& seq, bool feedback){
        std::vector<state_t> states(5);
        for(int i=0; i<5; ++i){
            states[i] = { 0, seq[i]-'0', false, input }; 
        }
        finished = false;
        result = 0;
        do{
            for(int i=0; i<seq.size(); ++i){
                run(states[i]);
            }
        }while(feedback && !finished);
        return result;
    };

    auto find_max_amp = [&](bool feedback){
        std::string seq = feedback ? "56789" : "01234";
        int mx = 0;
        do {
            mx = std::max(mx, amp_run(seq,feedback));
        } 
        while (std::next_permutation(seq.begin(),seq.end()));

        return mx;
    };

    std::cout << "part1: " << find_max_amp(false) << std::endl;
    std::cout << "part2: " << find_max_amp(true) << std::endl;
}