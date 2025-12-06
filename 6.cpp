#include <iostream>
#include <vector>
#include <string>

typedef long long ll;
using namespace std;

vector<pair<int, int>> parse_line_ends(const string& s) {
    vector<pair<int, int>> ends;
    string temp = ' ' + s + ' ';
    pair<int, int> current;
    for (int i = 1; i < (int)temp.size(); ++i) {
        if (isspace(temp[i]) != isspace(temp[i - 1])) {
            if (!isspace(temp[i])) {
                current.first = i - 1;
            } else {
                current.second = i - 2;
                ends.push_back(current);
            }
        }
    }
    return ends;
}

vector<ll> get_numbers(const string& s, const vector<pair<int, int>>& ends) {
    vector<ll> numbers;
    for (auto range : ends) {
        numbers.push_back(atoll(s.substr(range.first, range.second - range.first + 1).c_str()));
    }
    return numbers;
}

vector<char> get_operations(const string& s, const vector<pair<int, int>>& ends) {
    vector<char> result(ends.size());
    for (int i = 0; i < (int)ends.size(); ++i) {
        result[i] = s[ends[i].first];
    }
    return result;
}

ll add(ll x, ll y) {
    return x + y;
}
ll mul(ll x, ll y) {
    return x * y;
}

ll solve1(const vector<vector<ll>>& line_numbers, const vector<char>& operations) {
    ll result = 0;
    int n(line_numbers.size());
    int m(operations.size());
    
    for (int i = 0; i < m; ++i) {
        ll current = (operations[i] == '+' ? 0 : 1);
        auto operation = (operations[i] == '+' ? add : mul);
        for (int j = 0; j < n; ++j) {
            current = operation(current, line_numbers[j][i]);
        }
        result += current;
    }
    return result;
}

ll solve2(const vector<string>& lines, const vector<vector<pair<int, int>>>& line_ends, const vector<char>& operations) {
    ll result = 0;
    int n(lines.size());
    int m(operations.size());

    for (int i = 0; i < m; ++i) {
        ll current = (operations[i] == '+' ? 0 : 1);
        auto operation = (operations[i] == '+' ? add : mul);

        int mini = line_ends[0][i].first;
        int maxi = line_ends[0][i].second;
        for (int j = 1; j < n - 1; ++j) {
            mini = min(mini, line_ends[j][i].first);
            maxi = max(maxi, line_ends[j][i].second);
        }
        for (int l = mini; l <= maxi; ++l) {
            ll number = 0;
            for (int j = 0; j < n - 1; ++j) {
                if (isspace(lines[j][l])) {
                    continue;
                }
                number = number * 10LL + ll(lines[j][l] - '0');
            }
            current = operation(current, number);
        }
        result += current;
    }
    return result;
}
int main() {
    vector<string> lines;
    string line;
    vector<vector<pair<int, int>>> line_ends;
    while (getline(cin, line)) {
        lines.push_back(line);
        line_ends.push_back(parse_line_ends(line));
    }
    vector<vector<ll>> line_numbers;
    for (int i = 0; i < (int)line_ends.size() - 1; ++i) {
        line_numbers.push_back(get_numbers(lines[i], line_ends[i]));
    }
    vector<char> operations = get_operations(lines.back(), line_ends.back());

    cout << "Part 1: " << solve1(line_numbers, operations) << endl;
    cout << "Part 2: " << solve2(lines, line_ends, operations) << endl;
    return 0;
}