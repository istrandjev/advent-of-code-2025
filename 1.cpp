#include <iostream>

using namespace std;

int main() {
    string s;
    int current = 50;
    int answer1 = 0;
    int answer2 = 0;
    while (cin >> s) {
        int times = atoi(s.substr(1).c_str());
        if (s[0] == 'L') {
            times *= -1;
        }
        int new_current = current + times;
        answer2 += abs(new_current) / 100;
        answer2 += (new_current * current < 0);
        answer2 -= (new_current % 100 == 0 && new_current != 0);
        current = (new_current % 100 + 100) % 100;
        answer1 += (current == 0);
    }
    cout << "Part 1: " << answer1 << endl;
    cout << "Part 2: " << answer2 + answer1 << endl;
    return 0;
}