#include <iostream>
#include <set>
#include <map>

#define all(x) x.begin(), x.end()
using namespace std;
typedef long long ll;

struct point{
    ll x, y;
};

point parse_point(const string& s) {
    point result;
    sscanf(s.c_str(), "%lld,%lld", &result.x, &result.y);
    return result;
}


ll get_area(const point& a, const point& b) {
    return (abs(a.x - b.x) + 1) * (abs(a.y - b.y) + 1);
}

ll solve1(const vector<point>& points) {
    int n(points.size());
    ll answer = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            answer = max(answer, get_area(points[i], points[j]));
        }
    }
    return answer;
}

void connect(vector<vector<char>>& bitmap, const point& from, const point& to, const map<ll, int>& reverse_x, const map<ll, int>& reverse_y) {
    if (from.x == to.x) {
        int fy = reverse_y.find(from.y)->second;
        int ty = reverse_y.find(to.y)->second;
        int x = reverse_x.find(from.x)->second;
        for (int y = min(fy, ty) + 1; y <= max(fy, ty) - 1; ++y) {
            bitmap[x][y] = '-';
        }
        bitmap[x][fy] = bitmap[x][ty]= '+';
    } else {
        int fx = reverse_x.find(from.x)->second;
        int tx = reverse_x.find(to.x)->second;
        int y = reverse_y.find(from.y)->second;
        for (int x = min(fx, tx) + 1; x <= max(fx, tx) - 1; ++x) {
            bitmap[x][y] = '|';
        }
        bitmap[fx][y] = bitmap[tx][y] = '+';
    }
}

bool all_filled(const point& a, const point& b, const map<ll, int>& reverse_x, const map<ll, int>& reverse_y, const vector<vector<int>>& prefix_sums) {
    auto get_x = [&](int x){return reverse_x.find(x)->second;};
    auto get_y = [&](int y){return reverse_y.find(y)->second;};
    int ax = get_x(a.x);
    int ay = get_y(a.y);
    int bx = get_x(b.x);
    int by = get_y(b.y);
    int minx = min(ax, bx);
    int maxx = max(ax, bx);
    int miny = min(ay, by);
    int maxy = max(ay, by);
    int total = prefix_sums[maxx + 1][maxy + 1] + prefix_sums[minx][miny] - prefix_sums[minx][maxy + 1] - prefix_sums[maxx + 1][miny];
    return total == (maxx - minx + 1) * (maxy - miny + 1);
}

ll solve2(const vector<point>& points) {
    set<ll> xs, ys;
    for (auto p: points) {
        xs.insert(p.x);
        ys.insert(p.y);
    }
    vector<ll> x(all(xs)), y(all(ys));
    map<ll, int> reverse_x, reverse_y;
    for (int i = 0; i < (int)x.size(); ++i) {
        reverse_x[x[i]] = i;
    }

    for (int i = 0; i < (int)y.size(); ++i) {
        reverse_y[y[i]] = i;
    }

    vector<vector<char> > bitmap(xs.size() + 1, vector<char>(ys.size() + 1, 0));
    for (int i = 1; i < (int)points.size(); ++i) {
        connect(bitmap, points[i - 1], points[i], reverse_x, reverse_y);
    }
    connect(bitmap, points.back(), points[0], reverse_x, reverse_y);

    for (int i = 0; i < (int)bitmap.size(); ++i) {
        int opened = 0;
        for (int j = 0; j < (int)bitmap[i].size(); ++j) {
            if (bitmap[i][j] == '|') {
                opened++;
            } else if (bitmap[i][j] == '+') {
                if (i >= 1 && bitmap[i - 1][j] != 0 && bitmap[i - 1][j] != '*') {
                    opened++;
                }
            }
            if (bitmap[i][j] == 0 && opened % 2 == 1) {
                bitmap[i][j] = '*';
            }
        }
    }
    vector<vector<int>> prefix_sums(bitmap.size() + 1, vector<int>(bitmap[0].size() + 1, 0));
    for (int i = 0; i < (int)bitmap.size(); ++i) {
        for (int j = 0; j < (int)bitmap[i].size(); ++j) {
            prefix_sums[i + 1][j + 1] = (bitmap[i][j] != 0) + prefix_sums[i][j + 1] + prefix_sums[i + 1][j] - prefix_sums[i][j];
        }
    }
    
    int n(points.size());
    ll answer = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (all_filled(points[i], points[j], reverse_x, reverse_y, prefix_sums)) {
                answer = max(answer, get_area(points[i], points[j]));
            }
        }
    }
    return answer;
}
int main() {
    vector<point> points;
    string s;
    while (getline(cin, s)) {
        points.push_back(parse_point(s));
    }
    cout << "Part 1: " << solve1(points) << endl;
    cout << "Part 2: " << solve2(points) << endl;
    return 0;
}
