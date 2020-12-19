#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>

struct vec_t{
    int x,y,z;
};

struct moon_t{
    vec_t pos;
    vec_t vel;
};

vec_t& operator+=(vec_t& a,const vec_t& b) { 
    a.x += b.x; 
    a.y += b.y;  
    a.z += b.z; 
    return a; 
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
        vec_t pos {
            std::stoi(x.substr(0,x.find(","))),
            std::stoi(y.substr(0,y.find(","))),
            std::stoi(z.substr(0,z.find(">")))
        };
        vec_t vel { 0,0,0 };
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

        for(int y=0; y<moons.size(); ++y){
            for(int x=y; x<moons.size(); ++x){
                if(x != y){
                    if(moons[x].pos.x < moons[y].pos.x){
                        moons2[x].vel.x++;
                        moons2[y].vel.x--;
                    }
                    else if(moons[x].pos.x > moons[y].pos.x){
                        moons2[x].vel.x--;
                        moons2[y].vel.x++;
                    }
                    if(moons[x].pos.y < moons[y].pos.y){
                        moons2[x].vel.y++;
                        moons2[y].vel.y--;
                    }
                    else if(moons[x].pos.y > moons[y].pos.y){
                        moons2[x].vel.y--;
                        moons2[y].vel.y++;
                    }
                    if(moons[x].pos.z < moons[y].pos.z){
                        moons2[x].vel.z++;
                        moons2[y].vel.z--;
                    }
                    else if(moons[x].pos.z > moons[y].pos.z){
                        moons2[x].vel.z--;
                        moons2[y].vel.z++;
                    }
                }
            }
        }

        std::swap(moons,moons2);
    };

    auto apply_velocity = [&]{
        for(auto& moon : moons){
            moon.pos += moon.vel;
        }
    };

    auto energy = [](const vec_t& v){
        return std::abs(v.x) + std::abs(v.y) + std::abs(v.z);
    };

    //print_moons(moons);

    for(int s=0; s<1000; ++s){
        apply_gravity();
        apply_velocity();
        //print_moons(moons);
    }

    int total_energy = 0;
    for(auto& moon : moons){
        total_energy += energy(moon.pos) * energy(moon.vel);
    }
    std::cout << total_energy << std::endl;

}
