#include <iostream>
#include <set>
#include <string>
#include <vector>
#define all(x) x.begin(), x.end()


using namespace std;
typedef long long ll;
pair<set<int>, int> next_row(const set<int>& current, const string& next) {
    set<int> result;
    int hit = 0;
    for (auto beam: current) {
        if (next[beam] == '^') {
            result.insert(beam - 1);
            result.insert(beam + 1);
            hit++;
        } else {
            result.insert(beam);
        }
    }

    return {result, hit};
}

int solve1(const vector<string>& rows) {
    set<int> current;
    for (int i = 0; i < (int)rows[0].size(); ++i) {
        if (rows[0][i] == 'S') {
            current.insert(i);
            break;
        }
    }

    int answer = 0;
    for (int i = 0; i < (int)rows.size() - 1; ++i) {
        auto temp = next_row(current, rows[i + 1]);
        answer += temp.second;
        current = temp.first;
    }
    return answer;
}

ll mem[150][150];
#define update(x, y) (mem[x][y] == -1LL ? dp(x, y): mem[x][y])
vector<string> global_rows;

ll dp(int row, int idx) {
    if (row == (int)global_rows.size() - 1) {
        return mem[row][idx] = 1;
    }

    if (global_rows[row + 1][idx] != '^') {
        return mem[row][idx] = update(row + 1, idx);
    } else {
        return mem[row][idx] = update(row + 1, idx - 1) + update(row + 1, idx + 1);
    }
}
ll solve2(const vector<string>& rows) {
    memset(mem, -1, sizeof(mem));
    global_rows = rows;

    int start = -1;
    for (int i = 0; i < (int)rows[0].size(); ++i) {
        if (rows[0][i] == 'S') {
            start = i;
            break;
        }
    }
    return dp(0, start);
}
int main() {
    vector<string> rows;
    string s;
    while (getline(cin, s)) {
        rows.push_back(s);
    }
    
    cout << "Part 1: " << solve1(rows) << endl;
    cout << "Part 2: " << solve2(rows) << endl;
    return 0;
}
