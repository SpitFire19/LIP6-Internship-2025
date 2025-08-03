#include <iostream>
#include <vector>

#define DEBUG false

using namespace std;

using ll = long long;

template <typename T>
using matrix = vector<vector<T>>;

ll A = 0;
ll B = 10000;

int const G = 0;
int const O = 1;

// 7271 51809 86425 37460 9549 41488 22829 35448 82788 50846

// 7271 (51809 - 86425 + 37460) (9549 - 41488 + 22829) (35448 - 82788 + 50846)

// 7271 2844 -9110 3506

// val = 7271 - (3506 - (2844 - (-9110)))

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

// 1581 3691 591 6357 8825 4401 5250 4048 7177 5610 556 4009 8745 5773 698 2811

int main()
{
    srand(time(NULL));
    int n = 16;
    double total_mean = 0.0;

    vector<ll> lst = {0, 1};

    vector<ll> coins_initial = rand_coins(n, A, B);
    double min_mean = greedy_opt_ratio(coins_initial);

    vector<ll> min_coins(n), max_coins(n);
    for (;;)
    {
        vector<ll> coins = rand_coins_from_list(n, A, B, lst);
        double ratio = greedy_opt_ratio(coins);
        if (ratio < min_mean)
        {
            min_mean = ratio;
            cout << min_mean << endl;
            for(auto t : coins) {
                cout << t << " ";
            }
            cout << endl;
        }
    }
    
}