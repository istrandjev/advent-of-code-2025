#include <iostream> 
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

#define all(v) v.begin(),v.end()

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

vector<vector<int> > global_ne;
int target;
ll dfs(int v) {
    if (v == target) {
        return 1;
    }
    ll answer = 0;
    for (auto nxt : global_ne[v]) {
        answer += dfs(nxt);
    }
    return answer;
}
ll solve1(const vector<vector<int>>& ne, int from, int to) {
    int n = (int)ne.size();
    global_ne = ne;
    target = to;
    return dfs(from);    
}

vector<int> global_required;
map<pair<int, int>, ll > mem;
ll dfs2(int v, int mask) {
    auto it = mem.find({v, mask});
    if (it != mem.end()) {
        return it->second;
    }

    if (v == target) {
        ll result = (mask == ((1 << int(global_required.size())) - 1));
        mem[{v, mask}] = result;
        return result;
    }
    ll answer = 0;
    for (auto nxt : global_ne[v]) {
        int tmask = mask;
        for (int ri = 0; ri < (int)global_required.size(); ++ri) {
            if (global_required[ri] == nxt) {
                tmask |= (1 << ri);
            }
        }
        answer += dfs2(nxt, tmask);
    }
    mem[{v, mask}] = answer;
    return answer;
}

ll solve2(const vector<vector<int>>& ne, int from, int to, const vector<int>& required) {
    int n = (int)ne.size();
    global_ne = ne;
    target = to;
    global_required = required;
    return dfs2(from, 0);    
}

int main() {
    freopen("11.in", "r", stdin);
    unordered_map<string, int> node_codes;    
    auto get_node_code = [&](const string& s) {
        auto it = node_codes.find(s);
        if (it == node_codes.end()) {
            int result(node_codes.size());
            node_codes.insert({s, result});
            return result;
        } else {
            return it->second;
        }
    };

    string s;
    vector<vector<int>> ne;
    while (getline(cin, s)) {
        auto parts = split(s, ':');

        int from = get_node_code(parts[0]);
        while (from >= (int)ne.size()) {
            ne.push_back(vector<int>());
        }
        auto links = split(parts[1]);
        for (auto l : links) {
            ne[from].push_back(get_node_code(l));
        }
    }

    cout << "Part 1: " << solve1(ne, get_node_code("you"), get_node_code("out")) << endl;
    vector<int> required{
        get_node_code("dac"),
        get_node_code("fft"),
    };
    cout << "Part 2: " << solve2(ne, get_node_code("svr"), get_node_code("out"), required) << endl;
    return 0;
}