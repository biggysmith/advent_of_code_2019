#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

std::vector<int> parse_input(const std::string& file){
    std::ifstream file_stream(file);

    std::string line;
    std::getline(file_stream, line);

    std::vector<int> data(line.size());
    std::transform(line.begin(), line.end(), data.begin(), [](auto a){ return a - '0'; });

    return data;
}



int main()
{
    const auto input = parse_input("../src/day08/day8_input.txt");

    int width = 25;
    int height = 6;
    int depth = (int)input.size() / (width * height);

    {
        int min_layer;
        int min_zeros = INT_MAX;
        for (int z = 0; z < depth; ++z) {
            int zeros = 0;
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    if (input[(z*width*height) + (y*width + x)] == 0) {
                        zeros++;
                    }
                }
            }
            if (zeros < min_zeros) {
                min_zeros = zeros;
                min_layer = z;
            }
        }

        int num_1 = 0;
        int num_2 = 0;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (input[min_layer*width*height+y*width+x] == 1) {
                    num_1++;
                }
                else if (input[min_layer*width*height+y*width+x] == 2) {
                    num_2++;
                }
            }
        }

        std::cout << "part1: " << num_1 * num_2 << std::endl;
    }

    {
        std::vector<int> image = std::vector<int>(width*height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                for (int z = 0; z < depth; ++z) {
                    if(input[z*width*height+y*width+x] != 2){
                        image[y*width+x] = input[z*width*height+y*width+x];
                        break;
                    }
                }
            }
        }

        std::cout << "part2:" <<std::endl;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::cout << image[y*width+x];
            }
            std::cout << std::endl;
        }
    }
    
}