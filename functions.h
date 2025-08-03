/*
This is the file that contains basic includes, definitions,
utilities, functions, including those used to then calculate
moves under certain strategies

*/

#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::set;
using std::max;
using std::min;

using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::default_random_engine;

using ll = long long;
using ld = long double;

template <typename T>
using matrix = vector<vector<T>>;

ll A = 0;
ll B = 1000000;

int const G = 0;
int const O = 1;

bool const L = false;
bool const R = true;

double rand_double(double low, double high)
{
    int r = 1 + rand() % (INT32_MAX - 1);
    return (r * 1.0)/ INT32_MAX;
}

vector<ll> rand_coins(ll n, int A, int B)
{
    vector<ll> coins(n);
    for (ll &t : coins)
    {
        t = rand() % (B - A + 1); 
    }
    return coins;
}

vector<ll> rand_coins_from_list(ll n, vector<ll> vals)
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

matrix<bool> greedy_play(vector<ll> &coins, int l, int r)
{
    int n = r - l + 1;
    matrix<bool> greedy(n, vector<bool>(n + 1, false));
    for (int i = l; i <= r; ++i)
    {
        for (int j = i + 1; j <= r + 1; j++)
        {
            greedy[i - l][j - l] = coins[i] <= coins[j - 1];
        }
    }
    return greedy;
}

matrix<bool> optimal_play(vector<ll> &coins, int l, int r)
{
    ll n = r - l + 1;
    matrix<ll> dp(n, vector<ll>(n, 0));
    matrix<bool> opt(n, vector<bool>(n + 1, false));
    for (ll i = 0; i < n; ++i)
    {
        dp[i][i] = coins[i + l];
        opt[i][i + 1] = 1;
    }

    for (ll i = n; i >= 0; --i)
    {
        for (ll j = i + 1; j < n; ++j)
        {

            ll gain_left = coins[i + l] - dp[i + 1][j];
            ll gain_right = coins[j + l] - dp[i][j - 1];

            if (gain_left < gain_right)
            {
                dp[i][j] = gain_right;
                opt[i][j + 1] = R;
            }
            else if(gain_left == gain_right) {
                dp[i][j] = gain_left;
                opt[i][j + 1] = coins[i + l] < coins[j + l];
            }
            else
            {
                dp[i][j] = gain_left;
                opt[i][j + 1] = L;
            }
        }
    }
    return opt;
}

matrix<bool> optimal_vs_greedy_play(vector<ll> coins) {
    ll n = coins.size();
    vector<vector<ll>> dp(n, vector<ll> (n, 0));
    matrix<bool> opt_greedy(n, vector<bool> (n + 1, false));
    for(ll i = 0; i <n; ++i) {
        dp[i][i] = coins[i];
        opt_greedy[i][i + 1] = 1;
    }
    for(ll i = n - 1; i >=0; --i) {
        for(ll j = i + 1; j < n; ++j) {
            if((j - i) % 2 == 0) // Player 2's turn 
            {
                if(coins[i] < coins[j]) {
                    dp[i][j] = coins[j] - dp[i][j - 1];
                    opt_greedy[i][j + 1] = R;
                } else {
                    dp[i][j] = coins[i] - dp[i + 1][j];
                    opt_greedy[i][j + 1] = L;
                }
            }
            else {  // Player 1's turn
                ll gain_left = coins[i] - dp[i + 1][j];
                ll gain_right = coins[j] - dp[i][j - 1];

                if (gain_left < gain_right)
                {
                    dp[i][j] = gain_right;
                    opt_greedy[i][j + 1] = R;
                }
                else if(gain_left == gain_right) {
                    dp[i][j] = gain_left;
                    opt_greedy[i][j + 1] = coins[i] < coins[j];
                }
                else
                {
                    dp[i][j] = gain_left;
                    opt_greedy[i][j + 1] = L;
                }
            }
        }
    }
    return opt_greedy;
}

matrix<bool> optimal_vs_subopt_play(vector<ll> coins) {
    ll n = coins.size();
    vector<vector<ll>> dp(n, vector<ll> (n, 0));
    matrix<bool> opt_subopt(n, vector<bool> (n + 1, false));
    for(ll i = 0; i <n; ++i) {
        dp[i][i] = coins[i];
        opt_subopt[i][i + 1] = 1;
    }
    for(ll i = n - 1; i >=0; --i) {
        for(ll j = i + 1; j < n; ++j) {
            if((j - i) % 2 == 0) // Player 2's turn 
            {
                ll gain_left = coins[i] - dp[i + 1][j];
                ll gain_right = coins[j] - dp[i][j - 1];

                if (gain_left > gain_right)
                {
                    dp[i][j] = gain_right;
                    opt_subopt[i][j + 1] = R;
                }
                else if(gain_left == gain_right) {
                    dp[i][j] = gain_left;
                    opt_subopt[i][j + 1] = coins[i] < coins[j];
                }
                else
                {
                    dp[i][j] = gain_left;
                    opt_subopt[i][j + 1] = L;
                }
            }
            else {
                ll gain_left = coins[i] - dp[i + 1][j];
                ll gain_right = coins[j] - dp[i][j - 1];

                if (gain_left < gain_right)
                {
                    dp[i][j] = gain_right;
                    opt_subopt[i][j + 1] = R;
                }
                else if(gain_left == gain_right) {
                    dp[i][j] = gain_left;
                    opt_subopt[i][j + 1] = coins[i] < coins[j];
                }
                else
                {
                    dp[i][j] = gain_left;
                    opt_subopt[i][j + 1] = L;
                }
            }
        }
    }
    return opt_subopt;
}

ll gain_under_optimal_vs_greedy(vector<ll> coins) {
    ll n = coins.size();
    vector<vector<ll>> dp(n, vector<ll> (n, 0));

    for(ll i = 0; i < n; ++i) {
        dp[i][i] = coins[i];
    }
    for(ll i = n - 1; i >=0; --i) {
        for(ll j = i + 1; j < n; ++j) {
            if((j - i) % 2 == 0) // Player 2's turn 
            {
                if(coins[i] < coins[j]) {
                    dp[i][j] = coins[j] - dp[i][j - 1];
                } else {
                    dp[i][j] = coins[i] - dp[i + 1][j];
                }
            }
            else {
                dp[i][j] = max(coins[i] - dp[i+1][j],
                 coins[j] - dp[i][j-1]);
            }
        }
    }
    return dp[0][n-1];
}

ll gain_under_opt_vs_subopt(vector<ll> coins) {
    ll n = coins.size();
    vector<vector<ll>> dp(n, vector<ll> (n, 0));

    for(ll i = 0; i < n; ++i) {
        dp[i][i] = coins[i];
    }
    for(ll i = n - 1; i >=0; --i) {
        for(ll j = i + 1; j < n; ++j) {
            if((j - i) % 2 == 0) // Player 2's turn 
            {
                dp[i][j] = min(coins[i] - dp[i+1][j],
                 coins[j] - dp[i][j-1]);
            }
            else {
                dp[i][j] = max(coins[i] - dp[i+1][j],
                 coins[j] - dp[i][j-1]);
            }
        }
    }
    return dp[0][n-1];
}

ll gain_under_optimal_vs_greedy_ans(vector<ll> coins) {
    ll n = coins.size();
    vector<vector<ll>> dp(n, vector<ll> (n, 0));
    vector<vector<ll>> prev(n, vector<ll>(n, 0));
    for(ll i = 0; i < n; ++i) {
        dp[i][i] = coins[i];
        prev[i][i]= -1;
    }
    for(ll i = n - 1; i >=0; --i) {
        for(ll j = i + 1; j < n; ++j) {
            if((j - i) % 2 == 0) // Player 2's turn 
            {
                if(coins[i] < coins[j]) {
                    dp[i][j] = coins[j] - dp[i][j - 1];
                    prev[i][j] = j;
                } else {
                    dp[i][j] = coins[i] - dp[i + 1][j];
                    prev[i][j] = i;
                }
            }
            else {
                ll gain_left = coins[i] - dp[i + 1][j];
                ll gain_right = coins[j] - dp[i][j - 1];

                if (gain_left < gain_right)
                {
                    dp[i][j] = gain_right;
                    prev[i][j] = j;
                }
                else if(gain_left == gain_right) {
                    dp[i][j] = gain_left;
                    if(coins[i] < coins[j]) prev[i][j] = j;
                    else prev[i][j] = i;
                }
                else
                {
                    dp[i][j] = gain_left;
                    prev[i][j] = i;
                }
            }
        }
    }

    int first = 0, last = n - 1;
    while(prev[first][last] != -1) {
        int pr = prev[first][last];
        if(pr == first) {
            cout << pr << " ";
            first = pr + 1;
        } else {
            cout << pr << " ";
            last = pr - 1;
        }
    }
    cout << endl;

    return dp[0][n-1];
}

ll gain_under_optimal(vector<ll> coins) {
    ll n = coins.size();
    vector<vector<ll>> dp(n, vector<ll> (n, 0));
    for(ll i = 0; i <n; ++i) dp[i][i] = coins[i];
    for(ll i = n - 1; i >=0; --i) {
        for(ll j = i + 1; j < n; ++j) {
            dp[i][j] = max(coins[i] - dp[i+1][j],
                 coins[j] - dp[i][j-1]);
        }
    }
    return dp[0][n-1];
}