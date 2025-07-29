#include <iostream>
#include <vector>

#define DEBUG false

using namespace std;

using ll = long long;

template <typename T>
using matrix = vector<vector<T>>;

ll A = 0;
ll B = 1000000;

int const G = 0;
int const O = 1;

enum Strategy
{
    GREEDY,
    OPTIMAl
};

bool const L = false;
bool const R = true;

vector<ll> rand_coins(ll n, ll A, ll B)
{
    vector<ll> coins(n);
    bool non_zero = false;
    for (ll &t : coins)
    {
        t = rand() % (B + 1) - A;
        if (t != 0)
            non_zero = true;
    }
    if (!non_zero)
    {
        return rand_coins(n, A, B);
    }
    return coins;
}

vector<ll> rand_coins_from_list(ll n, ll A, ll B, vector<ll> vals)
{
    vector<ll> coins(n);
    int k = vals.size();
    bool non_zero = false;
    for (ll &t : coins)
    {
        int idx = rand() % k;
        t = vals[idx];
    }
    return coins;
}

matrix<bool> greedy_play(vector<ll> coins, bool dbg)
{
    int n = coins.size();
    matrix<bool> greedy(n, vector<bool>(n + 1, false));
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j <= n; j++)
        {
            greedy[i][j] = coins[i] <= coins[j - 1];
        }
    }
    if(dbg) {
    for(auto v : greedy) {
        for(auto t : v) {
            cout << t << ' ';
        }
        cout << endl;
    }
    }
    return greedy;
}

matrix<bool> optimal_play(vector<ll> coins, bool dbg)
{
    ll n = coins.size();
    matrix<ll> dp(n, vector<ll>(n, 0));
    matrix<bool> opt(n, vector<bool>(n + 1, false));
    auto gp = greedy_play(coins, false);
    for (ll i = 0; i < n; ++i)
    {
        dp[i][i] = coins[i];
        opt[i][i + 1] = 1;
    }

    for (ll i = n - 1; i >= 0; --i)
    {
        for (ll j = i + 1; j < n; ++j)
        {

            ll gain_left = coins[i] - dp[i + 1][j];
            ll gain_right = coins[j] - dp[i][j - 1];

            if (gain_left < gain_right)
            {
                dp[i][j] = gain_right;
                opt[i][j + 1] = R;
            }
            else if(gain_left == gain_right) {
                dp[i][j] = gain_left;
                opt[i][j + 1] = gp[i][j+1];
            }
            else
            {
                dp[i][j] = gain_left;
                opt[i][j + 1] = L;
            }
        }
    }
    if(dbg) {
    for(auto v : opt) {
        for(auto t : v) {
            cout << t <<  " ";
        }
        cout << endl;
    }
    cout << '\n';
    }
    return opt;
}

double greedy_opt_ratio(vector<ll> coins)
{
    int n = coins.size();
    matrix<bool> greedy = greedy_play(coins, false);
    // cout << endl;
    matrix<bool> optimal = optimal_play(coins, false);
    ll total = n * (n + 2)  / 4;
    ll count = 0;
    // 14 43 64 92
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n + 1; ++j)
        {
            if ((j - i) % 2 == 1 && greedy[i][j] == optimal[i][j])
                count++;
        }
    }
    return (count * 1.0) / total;
}

int main()
{
    srand(time(NULL));
    ll n = 100;
    ll iter = 100;
    double total_mean = 0.0;
    double max_mean = -1.0;
    double min_mean = 2;
    vector<ll> lst = {0, 1, 10000000};
    vector<ll> min_coins(n), max_coins(n);
    for (int it = 0; it < iter; ++it)
    {
        vector<ll> coins = rand_coins(n, A, B);
        double ratio = greedy_opt_ratio(coins);
        if (ratio < min_mean)
        {
            min_mean = ratio;
            copy(coins.begin(), coins.end(), min_coins.begin());
        }
        if (ratio > max_mean)
        {
            max_mean = ratio;
            copy(coins.begin(), coins.end(), max_coins.begin());
        }
        total_mean += ratio;
        cout << it << endl;
    }
    
    total_mean /= iter;
    cout << min_mean << endl;
    greedy_play(min_coins, false);
    cout << endl;
    optimal_play(min_coins, false);
    for (int t : min_coins)
    {
        cout << t << ' ';
    }
    cout << endl;
    /*
    cout << max_mean << endl;
    for (int t : max_coins)
    {
        cout << t << ' ';
    }
    cout << endl;
    */
    /*
    vector<ll> coins = {0, 0, 1000, 1};
    cout << greedy_opt_ratio(coins) << endl;
    */
    cout << "Mean ratio where greedy coincides with optimal is " << total_mean << endl;
}