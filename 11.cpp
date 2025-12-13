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
vector<int> global_required;
map<pair<int, int>, ll > mem;

set<pair<int, int> > path;
ll dfs(int v, int mask) {
    if (path.count({v, mask})) {
        return 0;
    }
    path.insert({v, mask});
    auto it = mem.find({v, mask});
    if (it != mem.end()) {
        path.erase({v, mask});
        return it->second;
    }

    if (v == target) {
        ll result = (mask == ((1 << int(global_required.size())) - 1));
        mem[{v, mask}] = result;
        path.erase({v, mask});
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
        answer += dfs(nxt, tmask);
    }
    mem[{v, mask}] = answer;
    path.erase({v, mask});
    return answer;
}

ll solve(const vector<vector<int>>& ne, int from, int to, const vector<int>& required) {
    int n = (int)ne.size();
    global_ne = ne;
    target = to;
    global_required = required;
    mem.clear();
    path.clear();
    return dfs(from, 0);    
}

int main() {
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

    cout << "Part 1: " << solve(ne, get_node_code("you"), get_node_code("out"), vector<int>()) << endl;
    cout << "Part 2: " << solve(ne, get_node_code("svr"), get_node_code("out"), {get_node_code("dac"), get_node_code("fft")}) << endl;
    return 0;
}