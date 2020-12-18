#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>

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

    //input.insert(input.end(),1<<10,0);

    return input;
}

struct state_t{
    std::vector<long long> prog;
    long long i;
    long long relative_base;

    int input;
    int output[2];
    bool done;
};

int mod(int a, int b) { return (a % b + b) % b; }

void main()
{
    auto input = parse_input("../src/day11/day11_input.txt");

    auto paint = [&](int start_color)
    {
        auto run = [&](state_t& state){
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
                    param(1) = state.input;
                    i += 2;
                }else if (op_code == 4) {
                    state.output[out_count] = (int)param(1);
                    out_count++;
                    i += 2;
                    if(out_count==2){
                        return true;
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
                    return false;
                }
            }

            return false;
        };

        int width = 200;
        int height = 200;
        std::vector<int> panels(width*height,0);

        auto print_panels = [&](int min_x, int min_y, int max_x, int max_y){
            for(int y=min_y-1; y<=max_y+1; ++y){
                for(int x=min_x-1; x<=max_x+1; ++x){
                    std::cout << panels[y*width+x];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        };

        state_t state;
        state.prog = input;
        state.i = 0;
        state.relative_base = 0;
        state.done = false;
        state.input = start_color;

        std::set<int> painted;
        int x = width/2;
        int y = height/2;
        int dir = 0;
        int min_x = INT_MAX;
        int max_x = INT_MIN;
        int min_y = INT_MAX;
        int max_y = INT_MIN;

        panels[y*width+x] = state.input;
        while(run(state)){
            painted.insert(y*width+x);

            panels[y*width+x] = state.output[0];

            //if(panels[y*width+x]){
                min_x = std::min(min_x, x);
                min_y = std::min(min_y, y);
                max_x = std::max(max_x, x);
                max_y = std::max(max_y, y);
            //}

            dir = mod(dir + (state.output[1] ? 1 : -1), 4);
            if(dir==0){
                y -= 1;
            }else if(dir==1){
                x += 1;
            }else if(dir==2){
                y += 1;
            }else if(dir==3){
                x -= 1;
            }

            state.input = panels[y*width+x];
        }

        std::cout << painted.size() << std::endl;
        print_panels(min_x, min_y, max_x, max_y);
    };

    paint(0);
    paint(1);

}
