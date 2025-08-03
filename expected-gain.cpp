#include "./functions.h"
#include <iomanip>  
using namespace std; 

using ll = long long;

template <typename T>
using matrix = vector<vector<T>>;

matrix<bool> greedy;
matrix<bool> opt;

vector<ll> rand_coins(ll n, ll A, ll B) {
    vector<ll> coins(n);
    for(ll &t : coins) {
        t = rand() % (B + 1) - A;
    }
    return coins;
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
    return dp[0][n-1];
}


// coins.size() has to be pair
double expected_gain(vector<ll> coins, int left, int right, double p, double q) {
    int n = right - left + 1;
    vector<matrix<double>> A(n,
         matrix<double> (n+1, vector<double> (2, 0.0)));

    for(int i = 0; i < n; ++i) {
        A[i][i][G] = 0.0;
        A[i][i][O] = 0.0;
        A[i][i+1][G] = -coins[i + left];
        A[i][i+1][O] = -coins[i + left];
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

                if(false) {
                    cout << p << "*" << "Gain from stay in greedy - " << same_state_gain << endl;
                    cout << (1-q) <<"*" << "Gain from tr from opt - " << change_state_gain << endl;
                }

                A[i][j][O] = p * same_state_gain + (1-p) * change_state_gain;

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
                } else {
                    double temp = A[i+1][j][G];
                    change_state_gain = -coins[i] + A[i+1][j][G];
                } 
                
                if(false) {
                    if(greedy[i][j] == opt[i][j]) {
                        cout << "Same strategy " << cts << endl;
                        ++cts;
                    }
                    if(same_state_gain == change_state_gain) {
                        cout << "SSame value " << ct << endl;
                        ++ct;
                    } else {
                        cout <<"DIFFERENT VALUE"<<endl;
                    }
                }

                A[i][j][G] = q * same_state_gain + (1-q) * change_state_gain;

            }
        }
    }
    if(false) {
        cout << "Finish at G: " << A[0][n][G] << endl;
        cout << "Finish at O: " << A[0][n][O] << endl;
    }
    return max(A[0][n][G], A[0][n][O]);
}


int main(void) {
    int n = 6;
    vector<ll> coins = {1, 10, 6, 4, 8, 2};
    double p, q;
    matrix<double> exp_g(11, vector<double> (11));
    /*
   
    for(int i = 0; i <= 10; ++i) {
         for(int j = 0; j <= 10; ++j) {
            exp_g[i][j] = expected_gain(coins, 0, n - 1, i * 0.1, j*0.1);
        }
    }
    */
    greedy = greedy_play(coins, 0, n - 1);
    opt = optimal_play(coins, 0, n - 1);
    cout.precision(6);
    cout << fixed;
    cout << "\t ";
    for(int j = 0;j <= 10; ++j){
        cout << j * 0.1 << " ";
    }
    cout << endl;
    for(int i = 0; i <= 10; ++i) {
        cout << i * 0.1 << " ";
         for(int j = 0;j <= 10; ++j) {
            cout << expected_gain(coins, 0, n - 1, i*0.1, j*0.1) << ' ';
        }
        cout << endl;
    }
    
    cin >> p >> q;
    cout << expected_gain(coins, 0, n - 1, p, q) << endl;
    cout << "Gain under optimal: " << gain_under_optimal(coins) << endl;
    return 0;
}