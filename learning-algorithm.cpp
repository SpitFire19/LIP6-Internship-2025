#include <iostream>
#include <vector>

using namespace std; 

using ll = long long;

template <typename T>
using matrix = vector<vector<T>>;

int const G = 0;
int const O = 1;

enum Strategy {
    GREEDY,
    OPTIMAl
};

bool const L = false;
bool const R = true;

vector<ll> rand_coins(ll n, ll A, ll B) {
    vector<ll> coins(n);
    for(ll &t : coins) {
        t = rand() % (B + 1) - A;
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

// coins.size() has to be pair
double expected_gain(vector<ll> coins, double p, double q, bool dbg) {
    int n = coins.size();
    vector<matrix<double>> A(n,
         matrix<double> (n+1, vector<double> (2, 0.0)));

    matrix<bool> greedy = greedy_play(coins, false);
    cout << endl;
    matrix<bool> opt = optimal_play(coins, false);

    for(int i = 0; i < n; ++i) {
        A[i][i][G] = 0.0;
        A[i][i][O] = 0.0;
        A[i][i+1][G] = -coins[i];
        A[i][i+1][O] = -coins[i];
    }
    if(dbg) {
        for(auto v : greedy) {
            for(auto t : v) {
                cout << t <<  " ";
            }
            cout << endl;
        }
        cout << "\n\n\n";
        for(auto v : opt) {
            for(auto t : v) {
                cout << t <<  " ";
            }
            cout << endl;
        }
    }
    int ct = 0;
    int cts = 0;
    for(int i = n - 1; i >= 0; --i) {
        for(int j = i + 2; j <= n; ++j) {
            // consider only plays of even length for 1st player
            // and plays of odd length for the 2nd player
            // the game is considered on [i, j)
            if((j - i) % 2 == 0) {
                A[i][j][G] = max(coins[i] + A[i+1][j][G], 
                                coins[j-1] + A[i][j-1][G]);
                A[i][j][O] = max(coins[i] + A[i+1][j][O], 
                                coins[j-1] + A[i][j-1][O]);
            }
            else {
                double same_state_gain = 0.0;
                if(greedy[i][j] == R) {
                    same_state_gain = -coins[j-1] + A[i][j-1][G];
                } else {
                    same_state_gain = -coins[i] + A[i+1][j][G];
                }
                double change_state_gain = 0.0;
                if(opt[i][j] == R) {
                    change_state_gain = -coins[j-1] + A[i][j-1][O];
                } else {
                    change_state_gain = -coins[i] + A[i+1][j][O];
                }

                if(dbg) {
                    cout << p << "*" << "Gain from stay in greedy - " << same_state_gain << endl;
                    cout << (1-q) <<"*" << "Gain from tr from opt - " << change_state_gain << endl;
               [page 47] }

                A[i][j][G] = p * same_state_gain + (1-p) * change_state_gain;

                if(dbg) {
                    if(greedy[i][j] == opt[i][j]) {
                        cout << "Same strategy " << cts << endl;
                        ++cts;
                    }
                    if(same_state_gain == change_state_gain) {
                        cout << "same value " << ct << endl;
                        ++ct;
                    } else {
               [page 47]         cout <<"DIFFERENT VALUE"<<endl;
                    }    
                }

                if(opt[i][j] == R) {
                    double temp = A[i][j-1][O];
                    same_state_gain = -coins[j-1] + A[i][j-1][O];
                } else {
                    double temp = A[i+1][j][O];
                    same_state_gain = -coins[i] + A[i+1][j][O];
                }

                if(greedy[i][j] == R) {
                    double temp = A[i][j-1][G];
                    change_state_gain = -coins[j-1] + A[i][j-1][G];
                } else {[page 47]
                    double temp = A[i+1][j][G];
                    change_state_gain = -coins[i] + A[i+1][j][G];
                } 
                
                if(dbg) {
                    if(greedy[i][j] == opt[i][j]) {
                        cout << "Same strategy " << cts << endl;
                        ++cts;
                    }
                    if(same_state_gain == change_state_gain) {
                        cout << "same value " << ct << endl;
                        ++ct;
                    } else {
                        cout <<"DIFFERENT VALUE"<<endl;
                    }
                }

                A[i][j][O] = q * same_state_gain + (1-q) * change_state_gain;

            }
        }
    }
    if(dbg) {
        cout << "Finish at G: " << A[0][n][G] << endl;
        cout << "Finish at O: " << A[0][n][O] << endl;
    }
    return max(A[0][n][G], A[0][n][O]);
}


