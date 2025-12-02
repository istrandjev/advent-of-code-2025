#include <iostream> 
#include <string>
#include <vector>
#include <set>
#include <numeric>

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

ll solve_part2(ll from, ll to, int from_times, int to_times) {
    set<ll> invalids;
    for (int times = from_times; times <= to_times; ++times) {
        for (ll pow10 = 10; pow10 < to; pow10 *= 10) {
            ll mul = pow10;
            ll base = 1;
            bool good = true;
            for (int i = 0; i < times - 1 && good; ++i) {
                base = base + mul;
                if (base > to) {
                    break;
                }
                mul *= pow10;
            }
            if (!good || base > to) {
                break;
            }

            ll max_val = to / base;
            ll min_val = from / base;

            if (min_val >= pow10) {
                continue;
            }
            if (min_val * base < from) {
                min_val++;
            }
            min_val = max(min_val, pow10 / 10);
            for (ll x = min_val; x * base <= to && x < pow10; ++x) {
                invalids.insert(x * base);
            }
        }
    }
    return accumulate(invalids.begin(), invalids.end(), 0LL);
}

int main() {
    string input;
    cin >> input;
    auto ranges = split(input, ',');
    ll answer1 = 0;
    ll answer2 = 0;
    for (const auto& range: ranges) {
        auto ends = split(range, '-');
        ll from = atoll(ends[0].c_str());
        ll to = atoll(ends[1].c_str());
        answer1 += solve_part2(from, to, 2, 2);
        answer2 += solve_part2(from, to, 2, 20);
    }
    cout << "Part 1: " << answer1 << endl;
    cout << "Part 2: " << answer2 << endl;
    return 0;
}