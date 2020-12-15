#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

std::vector<std::pair<std::string,std::string>> parse_input(const std::string& file){
    std::vector<std::pair<std::string,std::string>> input;

    std::ifstream file_stream(file);

    std::string line;
    while(std::getline(file_stream, line)){
        input.push_back({ line.substr(0,line.find(')')), line.substr(line.find(')')+1) });
    }

    return input;
}


int count_objects(const std::map<std::string,std::vector<std::string>>& orbits,const std::vector<std::string>& objects){
    int num_orbits = (int)objects.size();
    for(auto& object : objects){
        if(orbits.find(object) != orbits.end()){
            num_orbits += count_objects(orbits,orbits.at(object));
        }
    }
    return num_orbits;
};

void find_santa(const std::map<std::string,std::vector<std::string>>& orbits, const std::string& parent,const std::string& child, std::vector<int>& dists, int dist_travelled){

    if(child == "SAN"){
        dists.push_back(dist_travelled-1);
        return;
    }
    for(auto& object : orbits.at(child)){
        if(object != parent){ // no going back
            find_santa(orbits,child,object,dists,dist_travelled+1);
        }
    }

};

void main()
{
    auto input = parse_input("../src/day06/day6_input.txt");

    {
        std::map<std::string, std::vector<std::string>> orbits;
        for (auto& i : input) {
            orbits[i.second].push_back(i.first);
        }

        size_t num_orbits = 0;
        for (auto& orbit : orbits) {
            num_orbits += count_objects(orbits, orbit.second);
        }
        std::cout << "part1: " << num_orbits << std::endl;
    }

    {
        std::map<std::string, std::vector<std::string>> orbits;
        for (auto& i : input) {
            orbits[i.first].push_back(i.second);
            orbits[i.second].push_back(i.first);
        }

        std::vector<int> dists;
        find_santa(orbits,"YOU",orbits["YOU"].front(),dists,0);

        int min_dist = INT_MAX;
        for(auto d : dists){
            min_dist = std::min(min_dist, d);
        }
        std::cout << "part2: " << min_dist << std::endl;
    }
}