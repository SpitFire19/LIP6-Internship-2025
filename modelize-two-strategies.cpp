#include <iostream>
#include <vector>

using ll = long long;

using namespace std;

ll A = 0;
ll B = 10;

ll gain_under_parity(vector<ll> coins) {
    ll s_pair = 0, s_impair = 0;
    ll n = coins.size();
    for(ll i = 0; i < n; ++i) {
        if(i % 2 == 0) s_impair += coins[i];
        else s_pair += coins[i];
    }
    return max(s_pair, s_impair);
}

ll gain_under_optimal(vector<ll> coins) {
    ll n = coins.size();
    vector<vector<ll>> dp(n, vector<ll> (n, 0));
    for(ll i = 0; i <n; ++i) dp[i][i] = coins[i];
    ll sum = 0;
    for(ll i = 0; i <n; ++i) sum += coins[i];
    for(ll i = n - 1; i >=0; --i) {
        for(ll j = i + 1; j < n; ++j) {
            dp[i][j] = max(coins[i] - dp[i+1][j],
                 coins[j] - dp[i][j-1]);
        }
    }
    /*
    for(auto v : dp) { 
        for(auto t : v) cout << t << ' ';
        cout << endl;
    }
    */
    return (dp[0][n-1] + sum) / 2;
}

vector<ll> rand_coins(ll n, ll A, ll B) {
    vector<ll> coins(n);
    for(ll &t : coins) {
        t = rand() % (B + 1) - A;
    }
    return coins;
}

int main() {
    srand(time(NULL));
    ll n = 8;
    ll iter = 1000;

    double mx = -1;

    /*
    vector<ll> c = {2, 1000, 100, 1, 100, 3};
    cout << "Gain opt " << gain_under_optimal(c);
    cout << "Gain par " << gain_under_parity(c);
    cout << endl;
    */
    vector<ll> tmp;
    for(ll it = 0; it < iter; ++it) {
        vector<ll> coins = rand_coins(n, A, B);
        double gain_parity = (double) gain_under_parity(coins);
        double gain_opt = (double) gain_under_optimal(coins);
        double quotient = gain_opt / gain_parity;
        if(quotient > mx) {
            mx = quotient;
            tmp = coins;
        }
    }

    cout << "Maximal quotient optimal/parity is " << mx << endl;
    ll mn = 1e9;
    for(int i = 0; i < tmp.size(); ++i) {
        mn = min(mn, tmp[i]);
        //cout << tmp[i] << ' ';
    }
    for(int i = 0; i < tmp.size(); ++i) {
        cout << tmp[i] << ' ';
    }
    cout << endl;
    return 0;
}

