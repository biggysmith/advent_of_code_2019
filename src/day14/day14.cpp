#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <regex>
#include <chrono>

struct index_iterator {
    using difference_type = int64_t;
    using value_type = int64_t;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::random_access_iterator_tag;

    index_iterator(int64_t index) : index_(index) {};
    bool operator!=(const index_iterator& rhs) const { return index_ != rhs.index_; }
    const int64_t& operator*() const { return index_; }
    index_iterator& operator++() { index_++; return *this; }
    index_iterator& operator+=(difference_type n) { index_ += n; return *this; }
    difference_type operator-(const index_iterator& rhs) const { return index_ - rhs.index_; }

    int64_t index_;
};

struct reactant_t{
    int64_t quantity;
    std::string chemical;
};

struct reaction_t {
    std::vector<reactant_t> inputs;
    reactant_t output;
};

std::vector<reaction_t> parse_input(const std::string& file){
    std::vector<reaction_t> input;

    std::ifstream file_stream(file);
    std::string line;
    std::regex re("[0-9]+|[A-Z]+|=>");

    while(std::getline(file_stream, line)){

        std::regex re("[0-9]+|[A-Z]+|=>");
        std::sregex_token_iterator first{line.begin(), line.end(), re}, last;
        std::vector<std::string> tokens{first, last};

        reaction_t reaction;
        auto it = tokens.begin();
        while(*it != "=>"){
            reaction.inputs.push_back( { std::stoi(it[0]), it[1] } );
            it += 2;
        }

        it++;
        reaction.output = { std::stoi(it[0]), it[1] };
        input.push_back(reaction);

    }

    return input;
}

int64_t process(const std::vector<reaction_t>& reactions, std::map<std::string,int64_t>& extra, const reactant_t& reactant)
{
    if(reactant.chemical == "ORE"){
        return reactant.quantity;
    }

    auto it = std::find_if(reactions.begin(), reactions.end(), [&](const reaction_t& a){
        return a.output.chemical == reactant.chemical;
    });

    int64_t quantity_needed = reactant.quantity;

    if(extra[reactant.chemical]){ // already got some?
        int64_t tmp = quantity_needed;
        quantity_needed -= extra[reactant.chemical];
        extra[reactant.chemical] -= (tmp-quantity_needed);
    }

    int64_t quantity_per_reaction = it->output.quantity;
    int64_t qauntity_produced = (int64_t)std::ceil((float)quantity_needed/quantity_per_reaction);

    extra[reactant.chemical] += quantity_per_reaction * qauntity_produced - quantity_needed;
        
    int64_t ore = 0;
    for(auto& input : it->inputs){
        ore += process(reactions, extra, { input.quantity * qauntity_produced, input.chemical });
    }

    return ore;
}

void main()
{
    auto reactions = parse_input("../src/day14/day14_input.txt");

    auto ore_needed = [&](int64_t i){
        std::map<std::string, int64_t> extra;
        return process(reactions, extra, { i ,"FUEL" });
    };

    // part 1
    std::cout << "part1: " << ore_needed(1) << std::endl;

    // part 2
    auto it = std::lower_bound(index_iterator(0ll), index_iterator(3000000ll), 1000000000000ll, [&](int64_t a, int64_t b){
        return ore_needed(a) < b;
    });

    std::cout << "part2: " << *(it)-1 << std::endl;
}
