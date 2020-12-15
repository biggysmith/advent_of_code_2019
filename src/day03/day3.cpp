#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <map>

struct instruction{
    char cmd;
    int val;
};

struct wires{
    std::vector<instruction> wire_a;
    std::vector<instruction> wire_b;
};

wires parse_input(const std::string& file){
    wires input;

    std::ifstream file_stream(file);

    std::string line;

    {
        std::getline(file_stream, line);
        std::stringstream ss(line);
        std::string str;
        while (std::getline(ss, str, ',')) {
            input.wire_a.push_back({str[0], std::stoi(str.substr(1))});
        }
    }

    {
        std::getline(file_stream, line);
        std::stringstream ss(line);
        std::string str;
        while (std::getline(ss, str, ',')) {
            input.wire_b.push_back({str[0], std::stoi(str.substr(1))});
        }
    }

    return input;
}

struct point{
    int x,y;
};

struct line{
    point a,b;
};

bool operator!=(const point& a, const point& b){
    return !(a.x == b.x && a.y == b.y);
}

bool operator==(const point& a, const point& b){
    return a.x == b.x && a.y == b.y;
}

point operator-(const point& a, const point& b){
    return {a.x - b.x, a.y - b.y};
}

point operator+(const point& a, const point& b){
    return {a.x + b.x, a.y + b.y};
}

point operator*(const point& a, float b){
    return {(int)(a.x * b), (int)(a.y * b)};
}

int len(const point& a){
    return (int)sqrt(a.x*a.x + a.y*a.y);
}

int det(const point& a, const point& b){
    return a.x*b.y - b.x*a.y;
}

// probably overkill for 2d horizontal and vertical lines...
std::optional<point> intersect(const line& l0, const line& l1){
    point p0 = l0.b - l0.a;
    point p1 = l1.b - l1.a;

    int denom = det(p0,p1);

    if (denom == 0) {
        return std::nullopt;
    }

    point d = l0.a - l1.a;

    int s = det(p0, d);
    if(s < 0 == denom > 0) {
        return std::nullopt;
    }

    int t = det(p1, d);
    if (t < 0 == denom > 0){
        return std::nullopt;
    }

    if (s > denom == denom > 0 || t > denom == denom > 0){
        return std::nullopt;
    }

    float ft = t / (float)denom;

    return l0.a + p0 * ft;
}

void main()
{
    auto input = parse_input("../src/day03/day3_input.txt");

    auto get_lines = [](const std::vector<instruction>& wire){
        std::vector<line> lines;
        point pos{0,0};
        point ppos{0,0};
        for(int i=0; i<wire.size(); ++i){
            auto& c = wire[i];
            if(c.cmd == 'R'){
                pos.x += c.val;
            }else if(c.cmd == 'L'){
                pos.x -= c.val;
            }else if(c.cmd == 'U'){
                pos.y += c.val;
            }else if(c.cmd == 'D'){
                pos.y -= c.val;
            }
            lines.push_back({ppos,pos});
            ppos = pos;
        }
        return lines;
    };

    point zero{0,0};

    std::vector<point> crosses;
    std::map<int,std::vector<point>> line_a_crosses;
    std::map<int,std::vector<point>> line_b_crosses;

    auto lines_a = get_lines(input.wire_a);
    auto lines_b = get_lines(input.wire_b);

    for(int i=0; i<lines_a.size(); ++i){
        for(int j=0; j<lines_b.size(); ++j){
            auto p = intersect(lines_a[i],lines_b[j]);
            if(p && *p != zero){
                crosses.push_back(*p);
                line_a_crosses[i].push_back(*p);
                line_b_crosses[j].push_back(*p);
            }
        }
    }

    // part 1
    int mn = INT_MAX;
    point closest;
    for (auto& c : crosses) {
        int dist =  abs(c.x) + abs(c.y);
        if(dist < mn){
            mn = dist;
            closest = c;
        }
    }
    std::cout << "part1: " << mn << std::endl;

    // part 2
    auto wire_dist = [](const std::vector<line>& lines, const std::map<int,std::vector<point>>& line_crosses, point p){
        int idx = -1;
        for(auto& crosses : line_crosses){
            for(auto& cross : crosses.second){ // line can have multiple crosses along it
                if(cross == p){
                    idx = crosses.first;
                }
            }
        }

        int dist = 0;
        auto line = lines[idx];
        dist += len(p-line.a);
        for(int i=idx-1; i>=0; --i){
            line = lines[i];
            dist += len(line.b-line.a);
        }
        return dist;
    };

    int min_combined_dist = INT_MAX;
    for(auto& cross : crosses){
        int dist_a = wire_dist(lines_a, line_a_crosses, cross);
        int dist_b = wire_dist(lines_b, line_b_crosses, cross);

        min_combined_dist = std::min(min_combined_dist, dist_a + dist_b);
    }
    std::cout << "part2: " << min_combined_dist << std::endl;


}