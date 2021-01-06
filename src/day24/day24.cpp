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
#include <queue>

struct grid_t{
    std::vector<char> tiles;
    int width;
    int height;
};

grid_t parse_input(const std::string& file){
    grid_t input;

    std::ifstream file_stream(file);

    int y = 0;
    std::string line;
    while(std::getline(file_stream, line)){
        int x = 0;
        for(auto c : line){
            input.tiles.push_back(c);
            x++;
        }
        input.width = x;
        y++;
    }
    input.height = y;

    return input;
}


void print_grid(const grid_t& grid){
    for(int y=0; y<grid.height; ++y){
        for(int x=0; x<grid.width; ++x){
            std::cout << grid.tiles[y*grid.width+x];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

grid_t empty_grid(){
    std::vector<char> grid(25, '.');
    grid[2*5+2] = '?';
    return { grid, 5, 5 };
}

char& lookup(grid_t& g,int x,int y) {
    return g.tiles[y*g.width+x];
};

void update(grid_t& grid,grid_t& grid_copy){
    std::copy(grid.tiles.begin(), grid.tiles.end(), grid_copy.tiles.begin());

    for(int y=0; y<grid.height; ++y){
        for(int x=0; x<grid.width; ++x){

            int num_bugs = 
                (y>0 && lookup(grid,x,y-1) == '#') +
                (y<grid.height-1 && lookup(grid,x,y+1) == '#') +
                (x>0 && lookup(grid,x-1,y) == '#') +
                (x<grid.width-1 && lookup(grid,x+1,y) == '#');

            if(lookup(grid,x,y) == '#' && num_bugs!=1){
                lookup(grid_copy,x,y) = '.';
            }

            if(lookup(grid,x,y) == '.' && (num_bugs==1 || num_bugs==2)){
                lookup(grid_copy,x,y) = '#';
            }

        }
    }

    std::swap(grid, grid_copy);
};


int get_upper(std::vector<grid_t>& grid_stack, int x, int y, int level){
    int yy = y - 1;
    if(yy < 0){
        return lookup(grid_stack[level-1], 2, 1) == '#';
    }else if(x == 2 && yy == 2){
        return (lookup(grid_stack[level+1], 0, 4) == '#') +
               (lookup(grid_stack[level+1], 1, 4) == '#') +
               (lookup(grid_stack[level+1], 2, 4) == '#') +
               (lookup(grid_stack[level+1], 3, 4) == '#') +
               (lookup(grid_stack[level+1], 4, 4) == '#');
    }else{
        return lookup(grid_stack[level], x, yy ) == '#';
    }   
}

int get_right(std::vector<grid_t>& grid_stack, int x, int y, int level){
    int xx = x + 1;
    if(xx > 4){
        return lookup(grid_stack[level-1], 3, 2) == '#';
    }else if(xx == 2 && y == 2){
        return (lookup(grid_stack[level+1], 0, 0) == '#') +
               (lookup(grid_stack[level+1], 0, 1) == '#') +
               (lookup(grid_stack[level+1], 0, 2) == '#') +
               (lookup(grid_stack[level+1], 0, 3) == '#') +
               (lookup(grid_stack[level+1], 0, 4) == '#');
    }else{
        return lookup(grid_stack[level], xx, y ) == '#';
    }    
}

int get_lower(std::vector<grid_t>& grid_stack, int x, int y, int level){
    int yy = y + 1;
    if(yy > 4){
        return lookup(grid_stack[level-1], 2, 3) == '#';
    }else if(x == 2 && yy == 2){
        return (lookup(grid_stack[level+1], 0, 0) == '#') +
               (lookup(grid_stack[level+1], 1, 0) == '#') +
               (lookup(grid_stack[level+1], 2, 0) == '#') +
               (lookup(grid_stack[level+1], 3, 0) == '#') +
               (lookup(grid_stack[level+1], 4, 0) == '#');
    }else{
        return lookup(grid_stack[level], x, yy ) == '#';
    }    
}

int get_left(std::vector<grid_t>& grid_stack, int x, int y, int level){
    int xx = x - 1;
    if(xx < 0){
        return lookup(grid_stack[level-1], 1, 2) == '#';
    }else if(xx == 2 && y == 2){
        return (lookup(grid_stack[level+1], 4, 0) == '#') +
               (lookup(grid_stack[level+1], 4, 1) == '#') +
               (lookup(grid_stack[level+1], 4, 2) == '#') +
               (lookup(grid_stack[level+1], 4, 3) == '#') +
               (lookup(grid_stack[level+1], 4, 4) == '#');
    }else{
        return lookup(grid_stack[level], xx, y ) == '#';
    }    
}



void update_recursive(std::vector<grid_t>& grid_stack,std::vector<grid_t>& grid_stack_copy,int level)
{
    if(level == grid_stack.size()-1){
        return;
    }

    for(int y=0; y<grid_stack[level].height; ++y){
        for(int x=0; x<grid_stack[level].width; ++x){

            if(x==2 && y==2){
                update_recursive(grid_stack, grid_stack_copy, level+1);
            }

            int num_bugs = 0;
            num_bugs += get_upper(grid_stack, x, y, level);
            num_bugs += get_right(grid_stack, x, y, level);
            num_bugs += get_lower(grid_stack, x, y, level);
            num_bugs += get_left (grid_stack, x, y, level);

            if(lookup(grid_stack[level],x,y) == '#' && num_bugs!=1){
                lookup(grid_stack_copy[level],x,y) = '.';
            }

            if(lookup(grid_stack[level],x,y) == '.' && (num_bugs==1 || num_bugs==2)){
                lookup(grid_stack_copy[level],x,y) = '#';
            }

        }
    }

};

void update(std::vector<grid_t>& grid_stack,std::vector<grid_t>& grid_stack_copy){
    grid_stack_copy = grid_stack;

    update_recursive(grid_stack, grid_stack_copy, 1);

    std::swap(grid_stack, grid_stack_copy);
}


size_t bio_rating(const grid_t& grid){
    uint64_t i = 1;
    return std::accumulate(grid.tiles.begin(), grid.tiles.end(), 0ULL, [&](size_t a, auto& b){
        auto c = b=='#' ? i : 0;
        i <<= 1;
        return a + c;
    });
};

size_t count_bugs(const std::vector<grid_t>& grid_stack){
    return std::accumulate(grid_stack.begin(), grid_stack.end(), 0ULL, [](size_t a, auto& grid){
        return a + std::count(grid.tiles.begin(), grid.tiles.end(), '#');
    });
}


void main()
{
    {
        auto grid = parse_input("../src/day24/day24_input.txt");
        auto grid_copy = grid;

        std::map<uint64_t, int> tile_map;
        tile_map[bio_rating(grid)]++;

        while (true) {
            update(grid, grid_copy);
            uint64_t bio = bio_rating(grid);
            tile_map[bio]++;
            if (tile_map[bio] > 1) {
                std::cout << "part1: " << bio << std::endl;
                break;
            }
        }
    }


    {
        auto grid = parse_input("../src/day24/day24_input.txt");
        lookup(grid, 2, 2) = '?';

        int mins = 200;
        std::vector<grid_t> grid_stack(mins+4, empty_grid()); // +4 for extra slices at edges
        grid_stack[grid_stack.size()/2] = grid;
        std::vector<grid_t> grid_stack_copy = grid_stack;

        for(int i=0; i<mins; ++i){
            update(grid_stack, grid_stack_copy);
        }

        std::cout << "part2: " << count_bugs(grid_stack) << std::endl;
    }

}
