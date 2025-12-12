#include <iostream>
#include <string>
#include <vector>

#define all(x) x.begin(), x.end()

using namespace std;
typedef long long ll;

struct light {
    ll diagram;
    vector<ll> buttons;
    vector<int> joltage;
};
// For debugging purposes only
ostream& operator<<(ostream& out, const light& l) {
    int n(l.joltage.size());
    auto print_binary = [&](ll x) {
        for (int i = n - 1; i >= 0; --i) {
            out << (((x & (1 << i)) != 0) ? '1' : '0');
        }
    };
    print_binary(l.diagram);
    out << " [";
    for (auto b : l.buttons) {
        print_binary(b);
        out << ", ";
    }
    out << "] {";
    for (int i = 0; i < (int)l.joltage.size(); ++i) {
        if (i != 0) {
            out << ",";
        }
        out << l.joltage[i];
    }
    out << "}";
    return out;
}
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

ll parse_diagram(const string& s) {
    int n(s.size());
    ll result = 0;
    for (int i = n - 1; i >= 1; --i) {
        result *= 2LL;
        if (s[i] == '#') {
            result++;
        }
    }
    return result;
}

vector<int> parse_int_list(const string& s) {
    auto parts = split(s.substr(1, (int)s.size() - 2), ',');

    vector<int> result;
    for (auto part: parts) {
        result.push_back(atoi(part.c_str()));
    }
    return result;
}

ll get_button(const vector<int>& toggles) {
    ll result = 0;
    for (auto t : toggles) {
        result |= (1 << t);
    }
    return result;
}

light parse_light(const string& s) {
    auto parts = split(s);

    light result;
    result.diagram = parse_diagram(parts[0]);
    result.joltage = parse_int_list(parts.back());
    for (int i = 1; i < (int)parts.size() - 1; ++i) {
        result.buttons.push_back(get_button(parse_int_list(parts[i])));
    }
    return result;
}

int solve_light1(const light& l) {
    int n(l.buttons.size());
    int best = 1000;
    for (int mask = 0; mask < (1 << n); ++mask) {
        ll temp = 0;
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                temp ^= l.buttons[i];
                cnt++;
            }
        }
        if (temp == l.diagram) {
            best = min(best, cnt);
        }
    }
    return best;
}

const int IMPOSSIBLE = -2;

int solve1(const vector<light>& lights) {
    int result = 0;
    for (auto l : lights) {
        result += solve_light1(l);
    }
    return result;
}

long long evkl(long long a, long long b) {
    long long r;
    if (b > a)
        swap(b, a);
    if (a == 0) {
        if (b != 0)
            return b;
        else
            return 1;
    }
    if (b == 0)
        return a;
    while (a % b) {
        r = a % b;
        a = b;
        b = r;
    }
    return b;
}

struct GausRow {
    vector<ll> terms;
    ll value;
};

// For debugging purposes only
ostream& operator<<(ostream& out, const GausRow& e) {
    for (int i = 0; i < (int)e.terms.size(); ++i) {
        if (i != 0) {
            out << " + ";
        }
        out << e.terms[i] << " * " << char('a' + i);
    }
    out << " = " << e.value;
    return out;
}

struct GaussSolver {
    GaussSolver(int number_vars) : number_variables(number_vars) {}

    void add_equation(const vector<ll>& variables, int value) {
        GausRow r;
        r.terms.resize(number_variables, 0);
        r.value = value;
        for (auto v : variables) {
            r.terms[v] = 1;
        }
        rows.push_back(r);
    }

    void divide_row(int i, ll value) {
        for (int j = 0; j < rows[i].terms.size(); ++j) {
            rows[i].terms[j] /= value;
            
        }
        rows[i].value /= value;
    }

    void solve() {
        int current_column = 0;
        for (int i = 0; i < (int)rows.size() && current_column <= (int)rows[0].terms.size(); ++i, ++current_column) {
            int best = -1;
            for (int j = i; j < (int)rows.size(); ++j) {
                if (rows[j].terms[current_column] == 0) {
                    continue;
                }
                if (best == -1 || abs(rows[j].terms[current_column]) < abs(rows[best].terms[current_column])) {
                    best = j;
                }
            }
            if (best == -1) {
                i--;
                continue;
            } 
            swap(rows[i], rows[best]);
            if (rows[i].terms[current_column] < 0) {
                divide_row(i, -1);
            }
            for (int j = i + 1; j < (int)rows.size(); ++j) {
                if (rows[j].terms[current_column] == 0) {
                    continue;
                }
                ll d = evkl(abs(rows[i].terms[current_column]), abs(rows[j].terms[current_column]));

                ll mul1 = rows[j].terms[current_column] / d;
                ll mul2 = rows[i].terms[current_column] / d;
                for (int l = current_column; l < (int)rows[j].terms.size(); ++l) {
                    rows[j].terms[l] = rows[j].terms[l] * mul2 - rows[i].terms[l] * mul1;
                }
                rows[j].value = rows[j].value * mul2 - rows[i].value * mul1;
            }
            ll d = abs(rows[i].terms[current_column]);
            d = evkl(d, abs(rows[i].value));
            for (int j = current_column + 1; j < (int)rows[i].terms.size(); ++j) {
                d = evkl(d, abs(rows[i].terms[j]));
            }
            if (rows[i].terms[current_column] < 0) {
                d *= -1;
            }
            
            divide_row(i, d);
        }
    }
    int number_variables;
    vector<GausRow> rows;
};


GaussSolver global_gauss_solver(0);
vector<int> current_values;
vector<int> min_values;

int total_ways = 0;
int dfs(int button_index, int row_index) {
    if (button_index < 0) {
        total_ways++;
        return 0;
    }

    bool all_on_left_zero = true;
    for (int i = 0; i < button_index; ++i) {
        if (global_gauss_solver.rows[row_index].terms[i] != 0) {
            all_on_left_zero = false;
            break;
        }
    }
    if (all_on_left_zero && global_gauss_solver.rows[row_index].terms[button_index] == 0) {
        return dfs(button_index, row_index - 1);
    }

    if (all_on_left_zero) {
        int on_right = 0;
        for (int i = button_index + 1; i < (int)current_values.size(); ++i) {
            on_right += global_gauss_solver.rows[row_index].terms[i] * current_values[i];
        }

        int target = global_gauss_solver.rows[row_index].value - on_right;
        if (target % global_gauss_solver.rows[row_index].terms[button_index]) {
            return IMPOSSIBLE;
        } else {
            current_values[button_index] = target / global_gauss_solver.rows[row_index].terms[button_index];
            if (current_values[button_index] < 0) {
                return IMPOSSIBLE;
            }
            int tmp = dfs(button_index - 1, row_index - 1);
            if (tmp == IMPOSSIBLE) {
                return IMPOSSIBLE;
            }
            return current_values[button_index] + tmp;
        }
    } else {
        int result = IMPOSSIBLE;
        for (int i = 0; i <= min_values[button_index]; ++i) {
            current_values[button_index] = i;
            int temp = dfs(button_index - 1, row_index);
            if (temp != IMPOSSIBLE) {
                if (result == IMPOSSIBLE || result > temp + i) {
                    result = temp + i;
                }
            }
        }
        return result;
    }
}

int solve_light2(const light& l) {
    int n(l.joltage.size());
    int m(l.buttons.size());
    GaussSolver gs(l.buttons.size());
    for (int i = 0; i < n; ++i) {
        vector<ll> vars;
        int value = l.joltage[i];
        for (int j = 0; j < m; ++j) {
            if (l.buttons[j] & (1 << i)) {
                vars.push_back(j);
            }
        }
        gs.add_equation(vars, value);
    }
    
    gs.solve();
    global_gauss_solver = gs;
    min_values.clear();
    current_values.clear();
    min_values.resize(l.buttons.size(), 10000);
    current_values.resize(l.buttons.size(), 0);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (l.buttons[i] & (1 << j)) {
                min_values[i] = min(min_values[i], l.joltage[j]);
            }
        }
    }

    return dfs(m - 1, n - 1);
}


ll solve2(const vector<light>& lights) {
    ll result = 0;
    int idx = 0;
    for (auto l : lights) {
        result += solve_light2(l);
    }
    return result;
}

int main() {
    string s;
    vector<light> lights;
    while (getline(cin, s)) {
        lights.push_back(parse_light(s));
    }
    cout << "Part 1: " << solve1(lights) << endl;
    cout << "Part 2: " << solve2(lights) << endl;
    cout << "Part 3: " << total_ways << endl;
    
    return 0;
}