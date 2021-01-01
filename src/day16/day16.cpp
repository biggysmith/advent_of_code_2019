#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <execution>
#include <omp.h>

std::vector<int> parse_input(const std::string& file){
    std::vector<int> input;

    std::ifstream file_stream(file);

    std::string line;
    std::getline(file_stream, line);

    for(auto& c : line){
        input.push_back(c - '0');
    }

    return input;
}

int positive_last_digit(int n){
    return std::abs(n) % 10;
}

void main()
{
    {
        std::vector<int> input_signal = parse_input("../src/day16/day16_input.txt");
        std::vector<int> input_signal_copy(input_signal.size());

        std::vector<int> base_pattern{ 0, 1, 0, -1 };
        std::vector<int> pattern(input_signal.size(), 0);

        auto fill_pattern = [&](int element_n) {
            for (int i=0; i<pattern.size(); ++i) {
                pattern[i] = base_pattern[((i + 1) / element_n) % 4];
            }
        };

        auto apply_phase = [&] {
            for (int i=0; i<input_signal.size(); ++i) {
                fill_pattern(i + 1);
                input_signal_copy[i] = positive_last_digit(std::inner_product(input_signal.begin(), input_signal.end(), pattern.begin(), 0));
            }
            std::swap(input_signal, input_signal_copy);
        };

        for (int i=0; i<100; ++i) {
            apply_phase();
        }

        std::cout << "part1: ";
        for (int i=0; i<8; ++i) {
            std::cout << input_signal[i];
        }
        std::cout << std::endl;
    }



    {
        std::vector<int> input_signal = parse_input("../src/day16/day16_input.txt");

        auto apply_phase = [&](int offset) {
            int sum = 0;
            for(int i=(int)input_signal.size()-1; i>=0; --i){
                sum += input_signal[i];
                input_signal[i] = positive_last_digit(sum);
            }
        };

        std::string first7;
        for (int i = 0; i < 7; ++i) {
            first7 += '0' + input_signal[i];
        }
        int offset = std::stoi(first7);

        int size = (int)input_signal.size();
        input_signal.resize(10'000*size);
        for(int i=0; i<10000; ++i){
            std::copy(input_signal.begin(), input_signal.begin()+size, input_signal.begin()+i*size);
        }

        input_signal = std::vector<int>(input_signal.begin()+offset, input_signal.end());
   
        for (int i=0; i<100; ++i) {
            apply_phase(offset);
        }

        std::cout << "part2: ";
        for (int i=0; i<8; ++i) {
            std::cout << input_signal[i];
        }
        std::cout << std::endl;
    }


    
}
