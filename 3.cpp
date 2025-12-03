#include <iostream>
#include <string>

typedef long long ll;
using namespace std;

ll mem[128][20];
string global_s;
#define update(x, y) (mem[x][y] == -1 ? dp(x, y) : mem[x][y])

ll dp(int idx, int lft) {
    if (lft == 0) {
        return mem[idx][lft] = 0;
    }
    if (idx >= (int)global_s.size()) {
        return mem[idx][lft] = 0;
    }
    ll answer = 0;
    for (int i = idx; i < (int)global_s.size(); ++i) {
        ll temp = update(i + 1, lft - 1) * 10LL + (global_s[i] - '0');
        answer = max(answer, temp);
    }
    return mem[idx][lft] = answer;
}

ll joltage(const string& s, int times) {
    global_s = s;
    reverse(global_s.begin(), global_s.end());
    memset(mem, -1, sizeof(mem));
    return dp(0, times);
}

int main() {
    string s;
    ll answer1 = 0;
    ll answer2 = 0;
    while (getline(cin, s)) {
        answer1 += joltage(s, 2);
        answer2 += joltage(s, 12);
    }
    cout << "Part 1: " << answer1 << endl;
    cout << "Part 2: " << answer2 << endl;
    return 0;
}