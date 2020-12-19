#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <emmintrin.h>
#include <smmintrin.h>
#include <chrono>

struct timer{
    using timer_t = std::chrono::steady_clock;

    timer() 
        : s_(timer_t::now()) {}

    ~timer(){
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(timer_t::now() - s_).count() << "ms" << std::endl;
    }

    timer_t::time_point s_;
};

struct vec_t{
    union{
        struct{ int x,y,z,w; };
        __m128i data;
    };
    vec_t(const __m128i& v) : data(v){}
};

struct moon_t{
    vec_t pos;
    vec_t vel;
};

vec_t& operator+=(vec_t& a,const vec_t& b) { 
    return (a = _mm_add_epi32(a.data,b.data)); 
}

vec_t& operator-=(vec_t& a,const vec_t& b) { 
    return (a = _mm_sub_epi32(a.data,b.data));
}

vec_t operator-(const vec_t& a,const vec_t& b) { 
    return _mm_sub_epi32(a.data,b.data);
}

vec_t operator+(const vec_t& a,const vec_t& b) { 
    return _mm_add_epi32(a.data,b.data);
}

vec_t operator<(const vec_t& a,const vec_t& b) { 
    return _mm_cmplt_epi32(a.data,b.data);
}

vec_t operator>(const vec_t& a,const vec_t& b) { 
    return _mm_cmpgt_epi32(a.data,b.data);
}

bool operator==(const vec_t& a,const vec_t& b) { 
    __m128i x = _mm_xor_si128(a.data,b.data);
    return _mm_test_all_zeros(x,x);
}

bool operator==(const moon_t& a,const moon_t& b) { 
    return a.pos==b.pos && a.vel==b.vel;
}

vec_t xxxx(const vec_t& a) { 
    return _mm_shuffle_epi32(a.data,0x00);
}

vec_t yyyy(const vec_t& a) { 
    return _mm_shuffle_epi32(a.data,0x55);
}

vec_t zzzz(const vec_t& a) { 
    return _mm_shuffle_epi32(a.data,0xAA);
}

void print_moons(const std::vector<moon_t>& moons){
    for(auto& moon : moons){
        std::cout << "pos:" << moon.pos.x << "," << moon.pos.y << "," << moon.pos.z << " vel:" << moon.vel.x << "," << moon.vel.y << "," << moon.vel.z << std::endl;
    }
    std::cout << std::endl;
}

std::vector<moon_t> parse_input(const std::string& file){
    std::vector<moon_t> input;

    std::ifstream file_stream(file);

    std::string line;
    while(std::getline(file_stream, line)){
        std::string x = line.substr(line.find("x=")+2);
        std::string y = line.substr(line.find("y=")+2);
        std::string z = line.substr(line.find("z=")+2);
        vec_t pos = _mm_setr_epi32(
            std::stoi(x.substr(0,x.find(","))),
            std::stoi(y.substr(0,y.find(","))),
            std::stoi(z.substr(0,z.find(">"))),
            0
        );
        vec_t vel = _mm_setzero_si128();
        input.push_back({ pos, vel });
    }

    return input;
}

void main()
{
    auto moons = parse_input("../src/day12/day12_input.txt");
    auto moons2 = moons;

    auto apply_gravity = [&]{
        moons2 = moons;

        auto m0 = (moons[0].pos < moons[1].pos) - (moons[0].pos > moons[1].pos);
        auto m1 = (moons[0].pos < moons[2].pos) - (moons[0].pos > moons[2].pos);
        auto m2 = (moons[0].pos < moons[3].pos) - (moons[0].pos > moons[3].pos);
        auto m3 = (moons[1].pos < moons[2].pos) - (moons[1].pos > moons[2].pos);
        auto m4 = (moons[1].pos < moons[3].pos) - (moons[1].pos > moons[3].pos);
        auto m5 = (moons[2].pos < moons[3].pos) - (moons[2].pos > moons[3].pos);

        moons2[0].vel -= m0;
        moons2[1].vel += m0;
        moons2[0].vel -= m1;
        moons2[2].vel += m1;
        moons2[0].vel -= m2;
        moons2[3].vel += m2;
        moons2[1].vel -= m3;
        moons2[2].vel += m3;
        moons2[1].vel -= m4;
        moons2[3].vel += m4;
        moons2[2].vel -= m5;
        moons2[3].vel += m5;

        std::swap(moons,moons2);
    };

    auto apply_velocity = [&]{
        moons[0].pos += moons[0].vel;
        moons[1].pos += moons[1].vel;
        moons[2].pos += moons[2].vel;
        moons[3].pos += moons[3].vel;
    };

    auto energy = [](const vec_t& v){
        return std::abs(v.x) + std::abs(v.y) + std::abs(v.z);
    };

    
    {
        for (int s = 0; s < 1000; ++s) {
            apply_gravity();
            apply_velocity();
        }

        int total_energy = 0;
        for (auto& moon : moons) {
            total_energy += energy(moon.pos) * energy(moon.vel);
        }
        std::cout << "part1: " << total_energy << std::endl;
    }


    {
        moons = parse_input("../src/day12/day12_input.txt");
        moons2 = moons;
        auto orig_moons = moons;

        timer t;

        unsigned long long total_steps = 0;
        unsigned long long steps[3] = { 0,0,0 };

        do{
            apply_gravity();
            apply_velocity();
            total_steps++;

            if(!steps[0] && 
               xxxx(moons[0].pos) == xxxx(orig_moons[0].pos) && 
               xxxx(moons[0].vel) == xxxx(orig_moons[0].vel) &&
               xxxx(moons[1].pos) == xxxx(orig_moons[1].pos) && 
               xxxx(moons[1].vel) == xxxx(orig_moons[1].vel) &&
               xxxx(moons[2].pos) == xxxx(orig_moons[2].pos) && 
               xxxx(moons[2].vel) == xxxx(orig_moons[2].vel) &&
               xxxx(moons[3].pos) == xxxx(orig_moons[3].pos) && 
               xxxx(moons[3].vel) == xxxx(orig_moons[3].vel)
            ){
                steps[0] = total_steps;
            }

            if(!steps[1] && 
               yyyy(moons[0].pos) == yyyy(orig_moons[0].pos) && 
               yyyy(moons[0].vel) == yyyy(orig_moons[0].vel) &&
               yyyy(moons[1].pos) == yyyy(orig_moons[1].pos) && 
               yyyy(moons[1].vel) == yyyy(orig_moons[1].vel) &&
               yyyy(moons[2].pos) == yyyy(orig_moons[2].pos) && 
               yyyy(moons[2].vel) == yyyy(orig_moons[2].vel) &&
               yyyy(moons[3].pos) == yyyy(orig_moons[3].pos) && 
               yyyy(moons[3].vel) == yyyy(orig_moons[3].vel)
            ){
                steps[1] = total_steps;
            }

            if(!steps[2] && 
               zzzz(moons[0].pos) == zzzz(orig_moons[0].pos) && 
               zzzz(moons[0].vel) == zzzz(orig_moons[0].vel) &&
               zzzz(moons[1].pos) == zzzz(orig_moons[1].pos) && 
               zzzz(moons[1].vel) == zzzz(orig_moons[1].vel) &&
               zzzz(moons[2].pos) == zzzz(orig_moons[2].pos) && 
               zzzz(moons[2].vel) == zzzz(orig_moons[2].vel) &&
               zzzz(moons[3].pos) == zzzz(orig_moons[3].pos) && 
               zzzz(moons[3].vel) == zzzz(orig_moons[3].vel)
            ){
                steps[2] = total_steps;
            }

        }while(!(steps[0] && steps[1] && steps[2]));

        std::cout << "part2: " << std::lcm(std::lcm(steps[0],steps[1]),steps[2]) << std::endl;
    }


}
