#include <iostream>
#include <string>
#include <vector>

using namespace std;

int moves[8][2] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 },
        { 1, -1 }, { 0, -1 }, { -1, -1 } };

pair<int, vector<string> > solve1(const vector<string>& a, int neighbours) {
    int n(a.size());
    int m(a[0].size());

    int answer = 0;
    vector<string> modified(n, string(m, '.'));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int cnt = 0;
            if (a[i][j] != '@') {
                continue;
            }
            modified[i][j] = a[i][j];
            for (int l = 0; l < 8; ++l) {
                int tx = i + moves[l][0];
                int ty = j + moves[l][1];
                if (tx < 0 || ty < 0 || tx >= n || ty >= m) {
                    continue;
                }
                if (a[tx][ty] == '@') {
                    cnt++;
                }
            }
            if (cnt < neighbours) {
                answer++;
                modified[i][j] = '.';
            }
        }
    }
    return {answer, modified};
}

int do_count(const vector<string>& a) {
    int n(a.size());
    int m(a[0].size());
    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result += int(a[i][j] == '@');
        }
    }
    return result;
}

int main() {
    vector<string> a;
    string s;
    while (getline(cin, s)) {
        a.push_back(s);
    }
    auto result1 = solve1(a, 4);
    cout << "Part 1: " << result1.first << endl;
    while (result1.first) {
        result1 = solve1(result1.second, 4);
    }
    cout << "Part 2: " << do_count(a) -  do_count(result1.second) << endl;
    return 0;
}