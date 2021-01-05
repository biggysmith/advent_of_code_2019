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

struct packet_t{
    long long address,x,y;
};

struct computer_t{
    state_t state;

    int address;
    bool booted = false;
    std::queue<long long> input;
    std::vector<long long> output;
};



void run(computer_t& computer){
    auto& prog = computer.state.prog;

    long long& i = computer.state.i;
    long long& relative_base = computer.state.relative_base; 

    auto code = [&](long long p) -> auto&{
        if(p >= (long long )prog.size()){
            prog.insert(prog.end(), p+1-prog.size(), 0);
        }
        return prog[p];
    };

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
            if (!computer.booted) {
                param(1) = computer.address;
                computer.booted = true;
            }else {
                if (computer.input.empty()) {
                    return;
                }else {
                    param(1) = computer.input.front();
                    computer.input.pop();
                }
            }
            i += 2;
        }else if (op_code == 4) {
            computer.output.push_back(param(1));
            i += 2;
            if (computer.output.size() == 3){
                return;
            }
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




void main()
{
    auto input = parse_input("../src/day23/day23_input.txt");

    // part 1
    {
        std::vector<computer_t> computers(50);

        for (int i = 0; i < computers.size(); ++i) {
            computers[i].state = { input, 0, 0 };
            computers[i].address = i;
            computers[i].input.push(-1);
        }

        bool found = false;
        while (!found)
        {
            for (auto& c : computers) {
                run(c);

                if (c.output.empty()) {
                    continue;
                }

                if (c.output.front() == 255) {
                    std::cout << "part1: " << c.output[2] << std::endl;
                    found = true;
                    break;
                }

                for(int j=0; j<c.output.size(); j+=3){
                    auto packet = *reinterpret_cast<packet_t*>(c.output.data()+j);
                    computers[packet.address].input.push(packet.x);
                    computers[packet.address].input.push(packet.y);
                }
                c.output.clear();
            }
        }
    }


    // part 2
    {
        std::vector<computer_t> computers(50);

        for (int i = 0; i < computers.size(); ++i) {
            computers[i].state = { input, 0, 0 };
            computers[i].address = i;
            computers[i].input.push(-1);
        }

        int nat = 0;
        int x = 0;
        int y = 0;

        bool found = false;
        while (!found)
        {
            for (auto& c : computers) {
                run(c);

                if (c.output.empty()) {
                    continue;
                }

                if (c.output.front() == 255) {
                    auto packet = *reinterpret_cast<packet_t*>(c.output.data());
                    x = (int)packet.x;
                    y = (int)packet.y;
                    c.output.clear();
                    continue;
                }

                for(int j=0; j<c.output.size(); j+=3){
                    auto packet = *reinterpret_cast<packet_t*>(c.output.data()+j);
                    computers[packet.address].input.push(packet.x);
                    computers[packet.address].input.push(packet.y);
                }
                c.output.clear();
            }

            bool all_idle = true;
            for (auto& c : computers){
                if(!c.input.empty()){
                    all_idle = false;
                    break;
                }
            }

            if(all_idle)
            {
                if(y==0){
                    continue;
                }

                if (y == nat) {
                    std::cout << "part2: " << y << std::endl;
                    found = true;
                    break;
                }

                computers.front().input.push(x);
                computers.front().input.push(y);
                run(computers.front());
                nat = y;
            }

        }
    }


}
