#include "./functions.h"
#include "./strategies.h"
#include <chrono>
#pragma "O3"
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::set;
using std::max;
using std::min;
using std::pair;
using std::make_pair;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

template <typename T>
using matrix = vector<vector<T>>;

int const LOOK_AHEAD = 10;
int const TRANS_COUNT = 3;
int const TOTAL_COUNT = 100;

matrix<bool> opt_greedy;

Strategy *greedy;
Strategy *optimal;
Strategy* optimal_greedy;
Strategy* suboptimal;

vector<int> items;

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
                if(greedy->strategy_side(i, j - 1) == R) {
                    same_state_gain = -coins[j-1] + A[i][j-1][G];
                } else {
                    same_state_gain = -coins[i] + A[i+1][j][G];
                }
                double change_state_gain = 0.0;

                if(optimal->strategy_side(i, j - 1) == R) {
                    change_state_gain = -coins[j-1] + A[i][j-1][O];
                } else {
                    change_state_gain = -coins[i] + A[i+1][j][O];
                }

                A[i][j][O] = p * same_state_gain + (1-p) * change_state_gain;

                if(optimal->strategy_side(i, j - 1) == R) {
                    double temp = A[i][j-1][O];
                    same_state_gain = -coins[j-1] + A[i][j-1][O];
                } else {
                    double temp = A[i+1][j][O];
                    same_state_gain = -coins[i] + A[i+1][j][O];
                }

                if(greedy->strategy_side(i, j - 1) == R) {
                    double temp = A[i][j-1][G];
                    change_state_gain = -coins[j-1] + A[i][j-1][G];
                } else {
                    double temp = A[i+1][j][G];
                    change_state_gain = -coins[i] + A[i+1][j][G];
                }
                A[i][j][G] = q * same_state_gain + (1-q) * change_state_gain;

            }
        }
    }

    return max(A[0][n][G], A[0][n][O]);
}

pair<ll, bool> learning_algorithm(vector<ll> coins, Strategy* strategy) {
    // first move from opt_opt strategy
    int n = coins.size();
    Strategy* optimal = new OptimalStrategy(coins);
    Strategy* greedy  = new GreedyStrategy(coins);
    ll fp_gain = 0;
    ll sp_gain = 0;
    int left = 0, right = n - 1;
    State state = OPTIMAL, prev_state = UNDEFINED;
    int m = 0;  // total number of transitions
    int OO = 0, OG = 0, GO = 0, GG = 0;  // number of transitions of each type
    bool flag = false;

    for(int iter = 0; iter < n / 2; ++iter) {
        if((GG >= TRANS_COUNT && GO >= TRANS_COUNT 
            && OO >= TRANS_COUNT && OG >= TRANS_COUNT) || m >= TOTAL_COUNT) {
            double p_est = (OO * 1.0) / max(OO + OG, 1);
            double q_est = (GG * 1.0) / max(GG + GO, 1);
            if(!flag) {
                flag = true;
                cout << "Yes, ";
            }

            if(state == OPTIMAL) {  // Player 2 plays optimally
                int move = optimal->strategy(left, right);
                fp_gain += coins[move]; items.push_back(coins[move]);
                if(move == left) left++; else right--;
            }
            else if(state == GREEDY) {  // Player 2 plays greedily
                int move = optimal_greedy->strategy(left, right);
                fp_gain += coins[move]; items.push_back(coins[move]);
                if(move == left) left++; else right--;
            }
            else {  // Player 2's behaviour is undefined
                if(prev_state == GREEDY && q_est > 0.5) {
                    int move = optimal_greedy->strategy(left, right);
                    fp_gain += coins[move]; items.push_back(coins[move]);
                    if(move == left) left++; else right--;
                } else if(prev_state == OPTIMAL && p_est > 0.5) {
                    int move = optimal->strategy(left, right);
                    fp_gain += coins[move]; items.push_back(coins[move]);
                    if(move == left) left++; else right--;
                } else {
                    bool move_opt_greedy = optimal_greedy->strategy_side(left, right);
                    bool move_opt_opt = optimal->strategy_side(left, right);
                    if(move_opt_greedy == move_opt_opt) {
                        int move = optimal_greedy->strategy(left, right);
                        fp_gain += coins[move]; items.push_back(coins[move]);
                        if(move == left) left++; else right--;
                    } else {
                        int upper_left = min(left + LOOK_AHEAD, right);
                        int lower_right = max(left, right - LOOK_AHEAD);
                        double value_left = expected_gain(coins, left + 1, upper_left + 1,  p_est, q_est);
                        double value_right = expected_gain(coins, lower_right, right + 1, p_est, q_est);

                        if(value_left >= value_right) {
                            fp_gain += coins[left];
                            items.push_back(coins[left]);
                            left++;
                        } else {
                            fp_gain += coins[right];
                            items.push_back(coins[right]);
                            right--;
                        }
                    }
                }
            }
            
        } else {
            if(optimal->strategy(left, right) == left) {
                fp_gain += coins[left];
                items.push_back(coins[left]);
                left++;
            }
            else {
                fp_gain += coins[right];
                items.push_back(coins[right]);
                right--;
            }
        }

        State new_state;
        int p2move = strategy->strategy(left, right);
        if(optimal->strategy(left, right) != greedy->strategy(left, right)) {
            // an exact strategy was detected
            m++;
            if(p2move == optimal->strategy(left, right))
                new_state = OPTIMAL;
            else
                new_state = GREEDY;
        } else {
            new_state = UNDEFINED;
        }

        if(state == OPTIMAL && new_state == OPTIMAL) OO++;
        if(state == OPTIMAL && new_state == GREEDY)  OG++;
        if(state == GREEDY && new_state == OPTIMAL)  GO++;
        if(state == GREEDY && new_state == GREEDY)   GG++;

        if(p2move == left) {
            sp_gain += coins[left];
            left++;
        } else {
            sp_gain += coins[right];
            right--;
        }
        state = new_state;
        if(state != UNDEFINED) prev_state = state;
    }
    return make_pair(fp_gain - sp_gain, flag);
}

string random_obs(int n, double p_left, double p_right) {
    string observations;
    observations.resize(n / 2);
    for(int i = 0; i < observations.size(); ++i) {
        uniform_real_distribution<double> unif(0,1);
        default_random_engine re;
        double random_double = unif(re);

        if(random_double < p_left)
            observations[i] = 'L';
        else
            observations[i] = 'R';

    }
    return observations;
}

int main(void) {
    srand(time(0));
    int n;
    cin >> n;

    int num_iter = 10;
    int num_iter_learn = 0;
    double mean_ratio = 0.0;
    double mean_2 = 0.0;
    for(int it = 0; it < num_iter; ++it) {
    vector<ll> coins =  rand_coins(n, 0, 1000);

    opt_greedy = optimal_vs_greedy_play(coins);
    greedy = new GreedyStrategy(coins);
    optimal = new OptimalStrategy(coins);
    optimal_greedy = new OptGreedyStrategy(coins);
    auto opt_opt = gain_under_optimal(coins);
    ll p1gain_opt_g = gain_under_optimal_vs_greedy(coins);

    Strategy*player2strategy = new SubOptStrategy(coins);
    auto learn = learning_algorithm(coins, player2strategy);
    ll gain_learn = learn.first;

    ll gain_opt_opt = opt_opt;
    ll gain_subopt = gain_under_opt_vs_subopt(coins);
    cout << opt_opt << " " << gain_learn <<  " " << gain_subopt << endl;

        if(learn.second == true) {
            mean_ratio += (gain_learn * 1.0L) / opt_opt;
            mean_2 += (gain_learn * 1.0L) / gain_subopt;
            num_iter_learn++;
        }
    }

    cout << "Number of iter with learning algo: " << num_iter_learn << endl;
    cout << "Mean ratio learn/opt_opt is " << mean_ratio / num_iter_learn << endl;
    cout << "Mean ratio learn/opt_subopt is " << mean_2 / num_iter_learn << endl;
}

