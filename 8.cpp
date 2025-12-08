#include <iostream>
#include <vector>
#include <queue>
#define all(x) x.begin(), x.end()

using namespace std;

typedef long long ll;

class DSF {
public:
    DSF(int _n)
            : n(_n), dad(_n), rank(_n, 0) {
        for (int i = 0; i < (int)dad.size(); ++i) {
            dad[i] = i;
        }
    }

    int component(int a) {
        vector<int> sta;
        while (dad[a] != a) {
            sta.push_back(a);
            a = dad[a];
        }

        for (int i = 0; i < (int)sta.size(); ++i) {
            dad[sta[i]] = a;
        }

        return a;
    }

    pair<int, bool> merge(int a, int b) {
        vector<int> sta, stb;
        while (dad[a] != a) {
            sta.push_back(a);
            a = dad[a];
        }
        sta.push_back(a);

        while (dad[b] != b) {
            stb.push_back(b);
            b = dad[b];
        }
        stb.push_back(b);

        int nroot = a;
        if (rank[b] > rank[a]) {
            nroot = b;
        } else if (rank[b] == rank[a]) {
            rank[a]++;
        }
        for (int i = 0; i < (int)sta.size(); ++i) {
            dad[sta[i]] = nroot;
        }
        for (int i = 0; i < (int)stb.size(); ++i) {
            dad[stb[i]] = nroot;
        }

        return make_pair(nroot, a != b);
    }

private:
    int n;
    vector<int> dad;
    vector<int> rank;
};
ll square(ll x) {
    return x * x;
}
struct point {
    ll x, y, z;
    ll dist2(const point& o) const {
        
        return square(x - o.x) + square(y - o.y) + square(z - o.z);
    }
};

ostream& operator<<(ostream& out, const point& p) {
    out << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return out;
}


point parse_point(const string& s) {
    point result;
    sscanf(s.c_str(), "%lld,%lld,%lld", &result.x, &result.y, &result.z);
    return result;
}

vector<int> get_component_sizes(const vector<vector<int>>& ne) {
    int n(ne.size());
    vector<char> vis(n, 0);
    
    vector<int> result;
    
    for (int i = 0; i < n; ++i) {
        if (vis[i]) {
            continue;
        }
        int current_size = 1;
        queue<int> q;
        q.push(i);
        vis[i] = 1;
        while (!q.empty()) {
            int c = q.front();
            q.pop();
            for (auto nxt : ne[c]) {
                if (vis[nxt]) {
                    continue;
                }
                q.push(nxt);
                current_size++;
                vis[nxt] = 1;
            }
        }
        result.push_back(current_size);
    }
    return result;
}

pair<ll, ll> solve(const vector<point>& nodes, int limit = 1000) {
    int n(nodes.size());
    vector<pair<ll, pair<int, int >>> by_dist;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            by_dist.push_back({nodes[i].dist2(nodes[j]), {i, j}});
        }
    }
    sort(all(by_dist));
    DSF dsf(n);
    vector<vector<int>> ne(n);
    pair<ll, ll> result;
    for (int i = 0; i < (int)by_dist.size(); ++i) {
        
        auto link = by_dist[i].second;
        auto merge_result = dsf.merge(link.first, link.second);
        if (merge_result.second) {
            ne[link.first].push_back(link.second);
            ne[link.second].push_back(link.first);
            result.second = nodes[link.first].x * nodes[link.second].x;
        }
        
        if (i == limit - 1) {
            auto component_sizes = get_component_sizes(ne);
            sort(all(component_sizes));
            reverse(all(component_sizes));
            result.first = 1;
            for (int i = 0; i < 3; ++i) {
               result.first *= (ll)component_sizes[i];
            }
        }
    }
    
    return result;
}

int main() {
    string s;
    vector<point> nodes;
    while (getline(cin, s)) {
        nodes.push_back(parse_point(s));
    }
    auto result = solve(nodes, 1000);
    cout << "Part 1: " << result.first << endl;
    cout << "Part 2: " << result.second << endl;
    return 0;
}