#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(const string& s, char delim=' ') {
    vector<string> res;
    string cur;
    for (auto c : s) {
        if (c == delim) {
            res.push_back(cur);
            cur = "";
        } else {
            cur.push_back(c);
        }
    }
    if (s.back() != delim) {
        res.push_back(cur);
    }
    return res;
}

struct brick {
    vector<string> shape;

    int total_tiles() {
        int result = 0;
        for (int i = 0; i < (int)shape.size(); ++i) {
            for (int j = 0; j < (int)shape[i].size(); ++j) {
                result += (shape[i][j] == '#');
            }
        }
        return result;
    }
};

brick parse_brick() {
    string s;
    brick result;
    result.shape.resize(3);
    for (int i = 0; i < 3; ++i) {
        getline(cin, result.shape[i]);
    }
    return result;
}

struct puzzle {
    int w, h;
    vector<int> counts;
};

puzzle parse_puzzle(const string& s) {
    auto parts = split(s, ':');

    puzzle result;
    sscanf(parts[0].c_str(), "%dx%d", &result.w, &result.h);
    
    auto count_strings = split(parts.back().substr(1));
    for (auto count_string : count_strings) {
        result.counts.push_back(atoi(count_string.c_str()));
    }
    return result;
}

int main() {
    string s;
    vector<brick> bricks;
    vector<puzzle> puzzles;
    while (getline(cin, s)) {
        if (s.back() == ':') {
            bricks.push_back(parse_brick());
            getline(cin, s);
        } else {
            puzzles.push_back(parse_puzzle(s));
        }
    }
   
    int answer = 0;
    for (int pi = 0; pi < (int)puzzles.size(); ++pi) {
        const puzzle& p = puzzles[pi];

        int area = p.w * p.h;
        int total_tiles = 0;
        for (int i = 0; i < (int)p.counts.size(); ++i) {
            total_tiles += p.counts[i] * bricks[i].total_tiles();
        }
        if (total_tiles <= area) {
            answer++;
        }
    }
    cout << "Part 1: " << answer << endl;
    return 0;
}