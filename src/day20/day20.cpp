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

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <stack>

struct pos_t{
    int x,y;
};

bool operator<(const pos_t& a, const pos_t& b){
    return std::tie(a.x,a.y) < std::tie(b.x,b.y);
}

bool operator==(const pos_t& a, const pos_t& b){
    return std::tie(a.x,a.y) == std::tie(b.x,b.y);
}

struct maze_t{
    std::vector<char> grid;
    int width;
    int height;

    char get(const pos_t& p) const { 
        return grid[p.y*width+p.x]; 
    }

    void set(const pos_t& p,char c) { 
        grid[p.y*width+p.x] = c; 
    }
};

maze_t parse_input(const std::string& file){
    maze_t input;

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

void gotoxy(int x, int y) { 
    COORD pos = {(short)x, (short)y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

void print_maze(const maze_t& v){
    for(int y=0; y<v.height; ++y){
        for(int x=0; x<v.width; ++x){
            std::cout << v.grid[y*v.width+x];
        }
        std::cout << std::endl;
    }
}

pos_t begin_pos = {-1, -1};
pos_t end_pos = {-2, -2};

std::map<pos_t,pos_t> find_portals(const maze_t& m){
    std::map<std::string,std::vector<pos_t>> portals;

    for(int y=2; y<m.height-2; ++y){
        for(int x=2; x<m.width-2; ++x){

            if(m.get({x,y}) == ' ' || m.get({x,y}) == '#'){
                continue;
            }

            char up0 = m.get({x,y-1});
            char up1 = m.get({x,y-2});
            if(isupper(up0) && isupper(up1)){
                portals[std::string()+up1+up0].push_back({x,y});
            }

            char right0 = m.get({x+1,y});
            char right1 = m.get({x+2,y});
            if(isupper(right0) && isupper(right1)){
                portals[std::string()+right0+right1].push_back({x,y});
            }

            char down0 = m.get({x,y+1});
            char down1 = m.get({x,y+2});
            if(isupper(down0) && isupper(down1)){
                portals[std::string()+down0+down1].push_back({x,y});
            }

            char left0 = m.get({x-1,y});
            char left1 = m.get({x-2,y});
            if(isupper(left0) && isupper(left1)){
                portals[std::string()+left1+left0].push_back({x,y});
            }
        }
    }

    std::map<pos_t,pos_t> pos_map;
    for(auto& [port,pos] : portals){
        if(pos.size() == 2){
            pos_map[pos[0]] = pos[1];
            pos_map[pos[1]] = pos[0];
        }else if(port == "AA"){
            pos_map[begin_pos] = pos[0];
        }else if(port == "ZZ"){
            pos_map[end_pos] = pos[0];
        }
    }

    return pos_map;
    
}


void solve1(maze_t& m, const std::map<pos_t,pos_t>& pos_map, std::vector<int>& step_record, pos_t end, pos_t pos, int steps=0)
{
    if (pos == end){
        step_record.push_back(steps);
        return;
    }

    if(m.get(pos)!='@' && pos_map.count(pos)){ // stargate activate!
        m.set(pos, '@');
        m.set(pos_map.at(pos), '@');
        solve1(m, pos_map, step_record, end, pos_map.at(pos), steps+1);
    }

    m.set(pos, '@');

    if (m.get({pos.x-1,pos.y}) == '.'){
        solve1(m, pos_map, step_record, end, {pos.x-1,pos.y}, steps+1);
    }

    if (m.get({pos.x+1,pos.y}) == '.' ){
        solve1(m, pos_map, step_record, end, {pos.x+1,pos.y}, steps+1);
    }

    if (m.get({pos.x,pos.y-1}) == '.'){
        solve1(m, pos_map, step_record, end, {pos.x,pos.y-1}, steps+1);
    }

    if (m.get({pos.x,pos.y+1}) == '.'){
        solve1(m, pos_map, step_record, end, {pos.x,pos.y+1}, steps+1);
    }

    m.set(pos, '.');
}


void solve2(std::vector<maze_t>& maze_stack, const std::map<pos_t,pos_t>& pos_map, std::vector<int>& step_record, pos_t end, pos_t pos, int steps=0, int level=0)
{
    auto& m = maze_stack[level];

    if (level==0 && pos==end){
        step_record.push_back(steps);
        return;
    }

    if(m.get(pos)!='@' && pos_map.count(pos)){ // stargate activate!

        m.set(pos, '@');

        bool outer = (pos.x == 2 || pos.x==m.width-3 || pos.y == 2 || pos.y==m.height-3);

        if(outer) // going up
        { 
            if(level>0){
                maze_stack[level-1].set(pos_map.at(pos), '@');
                solve2(maze_stack, pos_map, step_record, end, pos_map.at(pos), steps+1, level-1);
            }
        }
        else // going down
        { 
            if(level+1 < maze_stack.size())
            {  
                maze_stack[level+1].set(pos_map.at(pos), '@');
                solve2(maze_stack, pos_map, step_record, end, pos_map.at(pos), steps+1, level+1);
            }
        }

    }

    m.set(pos, '@');

    if (m.get({pos.x-1,pos.y}) == '.'){
        solve2(maze_stack, pos_map, step_record, end, {pos.x-1,pos.y}, steps+1, level);
    }

    if (m.get({pos.x+1,pos.y}) == '.' ){
        solve2(maze_stack, pos_map, step_record, end, {pos.x+1,pos.y}, steps+1, level);
    }

    if (m.get({pos.x,pos.y-1}) == '.'){
        solve2(maze_stack, pos_map, step_record, end, {pos.x,pos.y-1}, steps+1, level);
    }

    if (m.get({pos.x,pos.y+1}) == '.'){
        solve2(maze_stack, pos_map, step_record, end, {pos.x,pos.y+1}, steps+1, level);
    }

    m.set(pos, '.');
}


void main()
{
    auto maze_orig = parse_input("../src/day20/day20_input.txt");
    auto pos_map = find_portals(maze_orig);

    {
        auto maze = maze_orig;
        std::vector<int> step_record;
        solve1(maze, pos_map, step_record, pos_map[end_pos], pos_map[begin_pos]);

        if(!step_record.empty()) {
            std::cout << "part1: " << *std::min_element(step_record.begin(), step_record.end()) << std::endl;
        }
    }

    {
        std::vector<maze_t> maze_stack(27, maze_orig); // needed to crank up the stack size for this one...
        std::vector<int> step_record;
        solve2(maze_stack, pos_map, step_record, pos_map[end_pos], pos_map[begin_pos]);

        if(!step_record.empty()) {
            std::cout << "part2: " << *std::min_element(step_record.begin(), step_record.end()) << std::endl;
        }
    }
}
