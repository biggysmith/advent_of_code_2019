#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

unsigned num_digits(unsigned i){
    return i > 0 ? (int)log10((double) i) + 1 : 1;
}

bool has_double_or_more_repeat_digit(const std::string& n){
    for(int i=1; i<n.size(); ++i){
        if(n[i]==n[i-1]){
            return true;
        }
    }
    return false;
}

bool has_double_repeat_digit(const std::string& n){
    for(unsigned i=1; i<n.size(); ++i){
        if(n[i-1] == n[i] &&
           n[i-1] != n[std::clamp(i-2,0U,(unsigned)n.size())] &&
           n[i]   != n[std::clamp(i+1,0U,(unsigned)n.size())] 
        ){
            return true;
        }
    }
    return false;
}

bool all_increasing(const std::string& n){
    for(int i=1; i<n.size(); ++i){
        if(n[i]<n[i-1]){
            return false;
        }
    }
    return true;
}

bool valid1(unsigned pass){
    std::string pass_str = std::to_string(pass);
    return num_digits(pass)==6 &&
           has_double_or_more_repeat_digit(pass_str) &&
           all_increasing(pass_str);
}

bool valid2(unsigned pass){
    std::string pass_str = std::to_string(pass);
    return num_digits(pass)==6 &&
           has_double_repeat_digit(pass_str) &&
           all_increasing(pass_str);
}

void main()
{
    unsigned mn = 246540;
    unsigned mx = 787419;

    {
        unsigned sum = 0;
        for (unsigned n=mn; n<mx; ++n) {
            if (valid1(n)) {
                sum++;
            }
        }
        std::cout << "part1: " << sum << std::endl;
    }

    {
        unsigned sum = 0;
        for (unsigned n = mn; n<mx; ++n) {
            if (valid2(n)) {
                sum++;
            }
        }
        std::cout << "part2: " << sum << std::endl;
    }
}