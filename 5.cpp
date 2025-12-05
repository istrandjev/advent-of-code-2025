#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

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

pair<ll, ll> get_range(const string& s) {
    auto parts = split(s, '-');
    return {atoll(parts[0].c_str()), atoll(parts[1].c_str())};
}

vector<pair<ll, ll> > get_ranges() {
    string line;
    vector<pair<ll, ll> > result;
    while (getline(cin, line)) {
        if (line.empty()) {
            break;
        }
        result.push_back(get_range(line));
    }

    return result;
}

vector<pair<ll, ll> > interval_union(vector<pair<ll, ll> > ranges) {
    sort(ranges.begin(), ranges.end());

    vector<pair<ll, ll>> result;
    pair<ll, ll> current = ranges[0];
    for (int i = 1; i < (int)ranges.size(); ++i) {
        if (ranges[i].first <= current.second + 1) {
            current.second = max(current.second, ranges[i].second);
            continue;
        }
        result.push_back(current);
        current = ranges[i];
    }

    result.push_back(current);
    return result;
}

int solve1(const vector<pair<ll, ll>>& ranges, const vector<ll>& foods) {
    int result = 0;
    for (auto food: foods) {
        bool good = false;
        for (auto range: ranges) {
            if (food >= range.first && food <= range.second) {
                good = true;
                break;
            }
        }
        result += good;
    }
    return result;
}

ll solve2(const vector<pair<ll, ll>>& ranges) {
    auto united = interval_union(ranges);
    ll total = 0;
    for (const pair<ll, ll>& range: united) {
        total += range.second - range.first + 1LL;
    }
    return total;
}

int main() {
    auto ranges = get_ranges();
    vector<ll> foods;
    ll food;
    while (cin >> food) {
        foods.push_back(food);
    }
    cout << "Part 1: " << solve1(ranges, foods) << endl;
    cout << "Part 2: " << solve2(ranges) << endl;
    return 0;
}