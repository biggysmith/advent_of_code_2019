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
#include "int128.h"

struct shuffle_t{
    int type;
    int n;
};

std::vector<shuffle_t> parse_input(const std::string& file){
    std::vector<shuffle_t> input;

    std::ifstream file_stream(file);

    std::string line;
    while(std::getline(file_stream, line)){
        if(line.find("deal into new stack") != std::string::npos){
            input.push_back({0,0});
        }else if(line.find("deal with increment ") != std::string::npos){
            input.push_back({ 1, std::stoi(line.substr(20)) });
        }else if(line.find("cut ") != std::string::npos){
            input.push_back({ 2,std::stoi(line.substr(4)) });
        }
    }

    return input;
}

void deal_into_new_stack(std::vector<uint64_t>& deck){
    std::reverse(std::begin(deck), std::end(deck));
}

void cut_n(std::vector<uint64_t>& deck, int64_t n){
    if(n < 0){
        std::rotate(std::rbegin(deck), std::rbegin(deck)-n, std::rend(deck));
    }else{
        std::rotate(std::begin(deck), std::begin(deck)+n, std::end(deck));
    }
}

void deal_with_increment(std::vector<uint64_t>& deck, int64_t n){
    std::vector<uint64_t> deck2(deck.size());
    std::swap(deck, deck2);
    for(uint64_t i=0, j=0; i<deck.size(); ++i, j=((j+n)%deck.size())){
        deck[j] = deck2[i];
    }
}

void print(const std::vector<uint64_t>& v){
    for(auto i : v){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


template <typename T>
T mod(T a, T b) { 
    return (a % b + b) % b; 
}

template <typename T>
T modmul(T a, T b, T mod) {
    T res = 0; 
    a = a % mod; 
    while (b > 0) { 
        if (b % 2 == 1) 
            res = (res + a) % mod; 
  
        a = (a * 2) % mod; 
        b /= 2; 
    } 
  
    return res % mod; 
} 

template <typename T>
T modpow(T base, T exp, T modulus) {
    base %= modulus;
    T result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
}

template <typename T>
T modinv(T n, T modulus) {
    return modpow(n, modulus-2, modulus);
}


void main()
{
    auto shuffles = parse_input("../src/day22/day22_input.txt");

    {
        std::vector<uint64_t> deck(10007);
        std::iota(deck.begin(), deck.end(), 0);

        for (auto& shuf : shuffles) {
            if (shuf.type == 0) {
                deal_into_new_stack(deck);
            }
            else if (shuf.type == 1) {
                deal_with_increment(deck, shuf.n);
            }
            else if (shuf.type == 2) {
                cut_n(deck, shuf.n);
            }
        }

        auto it = std::find(deck.begin(), deck.end(), 2019);
        std::cout << "part1: " << std::distance(deck.begin(), it) << std::endl;
    }

    {
        int128_t cards = 119315717514047;
        int128_t repeats = 101741582076661;

        int128_t increment = 1;
        int128_t offset = 0;

        for (auto& shuf : shuffles) {
            if (shuf.type == 0) {
                increment *= -1;
                increment = mod(increment, cards);
                offset += increment;
                offset = mod(offset, cards);
            }
            else if (shuf.type == 1) {
                int128_t n = shuf.n;
                increment *= modinv(n, cards);
                increment = mod(increment, cards);
            }
            else if (shuf.type == 2) {
                int128_t n = shuf.n;
                offset += n * increment;
                offset = mod(offset, cards);
            }
        }

        int128_t final_increment = modpow(increment, repeats, cards);
        int128_t final_offset = modmul(offset * (1-final_increment), modinv(mod(1-increment, cards), cards), cards);

        std::cout << "part2: " << mod(final_offset + 2020 * final_increment, cards) << std::endl;
    }

}
