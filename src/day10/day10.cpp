#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>

std::vector<std::string> parse_input(const std::string& file){
    std::vector<std::string> input;

    std::ifstream file_stream(file);

    std::string line;
    while(std::getline(file_stream, line)){
        input.push_back(line);
    }

    return input;
}

void main()
{
    auto input = parse_input("../src/day10/day10_input.txt");

    size_t width = input.front().size();
    size_t height = input.size();

    std::vector<int> output(width*height,0);

    for(int y=0; y<height; ++y){
        for(int x=0; x<width; ++x){
            if(input[y][x] == '#'){
                
                std::set<double> angles;
                for(int y2=0; y2<height; ++y2){
                    for(int x2=0; x2<width; ++x2){

                        if(x!=x2 || y!=y2){
                            int dx = x2-x;
                            int dy = y2-y;

                            if(input[y2][x2] == '#'){
                                angles.insert(atan2(dx,dy));
                            }
                        }

                    }
                }

                output[y*width+x] = (int)angles.size();

            }
        }
    }

    int mx = 1;
    int bx = -1;
    int by = -1;
    for(int y=0; y<height; ++y){
        for(int x=0; x<width; ++x){
            if(output[y*width+x] > mx){
                mx = output[y*width+x];
                bx = x;
                by = y;
            }
        }
    }

    std::cout << "(" << bx << "," << by << "): " << mx << std::endl;
}
