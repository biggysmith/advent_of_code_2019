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
    while (std::getline(file_stream, line)) {
        input.push_back(std::stoi(line));
    }

    return input;
}

void main()
{
    auto input = parse_input("../src/day01/day1_input.txt");

    // part 1
    {
        int sum = std::accumulate(input.begin(), input.end(), 0, [](auto a, auto b) {
            return a + (b/3 - 2);
        });
        std::cout << sum << std::endl;
    }

    // part 2
    {
        int sum = std::accumulate(input.begin(), input.end(), 0, [](auto a, auto b) {
            int module_fuel = b/3 - 2;
            int fuel_fuel = module_fuel/3 - 2;
            while (fuel_fuel > 0) {
                module_fuel += fuel_fuel;
                fuel_fuel = fuel_fuel/3 - 2;
            }
            return a + module_fuel;
        });
        std::cout << sum << std::endl;
    }

}