#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <chrono>
#include <iterator>
#include <queue>
#include <nmmintrin.h>
#include <unordered_set>

struct pos_t{
    int x,y,z;
    int steps;
};


namespace std {
    template <> struct hash<pos_t> {
        std::size_t operator()(const pos_t& pos) const {
            return ((hash<int>()(pos.x) ^ (hash<int>()(pos.y) << 1)) >> 1) ^ (hash<int>()(pos.z) << 1);
        }
    };
}

bool operator==(const pos_t& a,const pos_t& b){
    return a.x == b.x && a.y == b.y && a.z == b.z;
}


struct vault_t{
    std::vector<char> grid;
    int width;
    int height;
};

vault_t parse_input(const std::string& file){
    vault_t input;

    std::ifstream file_stream(file);

    int y = 0;
    std::string line;
    while(std::getline(file_stream, line)){
        input.width = (int)line.size();
        for(auto c : line){
            input.grid.push_back(c);
        }
        y++;
    }
    input.height = y;

    return input;
}

void print_vault(const vault_t& v){
    for(int y=0; y<v.height; ++y){
        for(int x=0; x<v.width; ++x){
            std::cout << v.grid[y*v.width+x];
        }
        std::cout << std::endl;
    }
}

pos_t find_entrance(const vault_t& v){
    auto it = std::find(v.grid.begin(), v.grid.end(), '@');
    if(it != v.grid.end()){
        int d = (int)std::distance(v.grid.begin(), it);
        return { d % v.width, d / v.width };
    }else{
        return { -1, -1 };
    }
}

std::set<char> find_keys(const vault_t& v){
    std::set<char> ret;
    std::copy_if(v.grid.begin(), v.grid.end(), std::inserter(ret, ret.begin()), islower);
    return ret;
}

int count_keys(const vault_t& v){
    return (int)std::count_if(v.grid.begin(), v.grid.end(), islower);
}

int to_index(char c){
    return c - 97;
}

int count(int mask){
    return _mm_popcnt_u32(mask);
}

bool is_set(int mask,int bit){
    return mask & (1 << bit);
}

int set(int mask,int bit){
    return mask |= 1 <<  bit;
}

int unset(int mask,int bit){
    return mask &= ~(1 << bit); 
}

int find_start_z(const vault_t& v, std::set<char> keys, int bx, int ex, int by, int ey){
    for(int y=by; y<ey; ++y){
        for(int x=bx; x<ex; ++x){
            char c = v.grid[y*v.width+x];
            if(islower(c)){
                keys.erase(c);
            }
        }
    }

    int z = 0;
    for(auto c : keys){
        z = set(z, to_index(c));
    }
    return z;
};

int breadth_first_search(const vault_t& v, pos_t pos)
{
    int goal = count_keys(v);
    std::unordered_set<pos_t> visited;

    std::queue<pos_t> q;
    q.push({pos.x,pos.y,pos.z,0});

    while (!q.empty())
    {
        pos = q.front();
        q.pop();

        if(count(pos.z) == goal){
            return pos.steps;
        }

        for(int i=0; i<4; ++i)
        {
            pos_t npos = pos;
            if(i==0){
                npos.y--;
            }else if(i==1){
                npos.x++;
            }else if(i==2){
                npos.y++;
            }else if(i==3){
                npos.x--;
            }

            char c = v.grid[npos.y*v.width+npos.x];

            if(c == '#' || visited.count(npos)){
                continue;
            }

            if(isupper(c) && !(is_set(pos.z,to_index(tolower(c))))) {  
                continue; 
            }

            npos.steps++;
            if(islower(c)){
                npos.z = set(npos.z,to_index(c));
            }
            q.push(npos);
            visited.insert(npos);

        }
    }

    return -1;
}

void main()
{
    auto vault = parse_input("../src/day18/day18_input.txt");

    auto pos = find_entrance(vault);

    std::cout << "part1: " << breadth_first_search(vault, pos) << std::endl;

    auto val = [&](int x,int y) -> auto& {
        return vault.grid[y*vault.width+x];
    };

    val(pos.x-1, pos.y-1) = '@'; val(pos.x, pos.y-1) = '#'; val(pos.x+1, pos.y-1) = '@';
    val(pos.x-1, pos.y  ) = '#'; val(pos.x, pos.y  ) = '#'; val(pos.x+1, pos.y  ) = '#';
    val(pos.x-1, pos.y+1) = '@'; val(pos.x, pos.y+1) = '#'; val(pos.x+1, pos.y+1) = '@';

    pos_t pos0 = { pos.x-1, pos.y-1 };
    pos_t pos1 = { pos.x+1, pos.y-1 };
    pos_t pos2 = { pos.x+1, pos.y+1 };
    pos_t pos3 = { pos.x-1, pos.y+1 };

    auto keys = find_keys(vault);
    pos0.z = find_start_z(vault, keys, 0, pos0.x+1, 0, pos0.y+1);
    pos1.z = find_start_z(vault, keys, pos1.x, vault.width, 0, pos1.y+1);
    pos2.z = find_start_z(vault, keys, pos3.x, vault.width, pos3.y, vault.height);
    pos3.z = find_start_z(vault, keys, 0, pos2.x+1, pos2.y, vault.height);

    int steps = 0;
    steps += breadth_first_search(vault, pos0);
    steps += breadth_first_search(vault, pos1);
    steps += breadth_first_search(vault, pos2);
    steps += breadth_first_search(vault, pos3);

    std::cout << "part2: " << steps << std::endl;

}