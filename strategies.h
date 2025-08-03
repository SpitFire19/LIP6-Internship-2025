#pragma once
#include "./functions.h"

enum State
{
    GREEDY,
    OPTIMAL,
    UNDEFINED
};

/*
Given an array of coins, we would like to answer
the following request in O(1) time:
What position (what side) to play on interval
[left, right] under a certain strategy ?
*/

class Strategy {
    protected:
        vector<long long> coins;
    public:
    Strategy() {}
    Strategy(vector<long long> &coin_row) {
        this->coins = coin_row;
    }
    virtual int strategy(int left, int right) = 0;
    bool strategy_side(int left, int right) {
            if(this->strategy(left, right) == left)
                return L;
            else 
                return R;
        }
};

/*
Under greedy strategy, each player takes the highest of two
values on the sides of array of coins
*/
class GreedyStrategy : public Strategy {
    public:
        GreedyStrategy(vector<ll>& coins) : Strategy(coins) {}
        int strategy(int left, int right) override{
            if(coins[left] <= coins[right])
                return right;
            else
                return left;
        }
        
};

/*
Each player is playing optimally, trying to maximize his gain
*/
class OptimalStrategy : public Strategy {
    private:
        matrix<bool> opt_opt;
    public:
        OptimalStrategy(vector<ll>& coins) : Strategy(coins) {
            this->opt_opt = optimal_play(coins, 0, this->coins.size() - 1);
        }
        int strategy(int left, int right) override{
            if(opt_opt[left][right + 1] == L)
                return left;
            else
                return right;
        }
};

/*
The first player is playing optimally
The second player is playing to minimize his gain
*/
class SubOptStrategy : public Strategy {
    private:
        matrix<bool> opt_subopt;
    public:
        SubOptStrategy(vector<ll>& coins) : Strategy(coins) {
            this->opt_subopt = optimal_vs_subopt_play(coins);
        }
        int strategy(int left, int right) override {
            if(this->opt_subopt[left][right + 1] == L) {
                return left;
            }
            else {
                return right;
            }
        }
};

/*
The first player is playing optimally
The second player is playing greedily
*/
class OptGreedyStrategy : public Strategy {
    private:
        matrix<bool> opt_greedy;
    public:
        OptGreedyStrategy(vector<ll> &coins) : Strategy(coins) {
            this->opt_greedy = optimal_vs_greedy_play(coins);
        }
        int strategy(int left, int right) override {
            if(this->opt_greedy[left][right + 1] == L)
                return left;
            else
                return right;
        }
};

/*
    Under Markov Strategy, Player 2 (only he is considered)
    is modelized as a Markov chain with two states: O and G, and
    with two transition probabilities, p and q. The transitions are
    as follows:
    O->O with Pr = p
    G->G with Pr = q
    O->G with Pr = 1-p
    G->O with Pr = 1-q
    After playing greedily or optimally, depending on his state, Player 2
    transitions to another state in the way indicated above.
*/
class MarkovStrategy : public Strategy {
 
    private:
        GreedyStrategy* greedy;
        OptimalStrategy* opt_opt;
        double p;
        double q;
        State state;
    public:
        MarkovStrategy(vector<ll>&coins, double p, double q) : Strategy(coins) {
            this->p = p;
            this->q = q;
            this->state = OPTIMAL;
            this->greedy = new GreedyStrategy(coins);
            this->opt_opt = new OptimalStrategy(coins);
        }
        void setState(State newState) {
            this->state = newState;
        }
        State getState() const {
            return this->state;
        }
        int strategy(int left, int right) override{
            double d = rand_double(0, 1);
            if(this->state == OPTIMAL) { // Make optimal move
                 if(d < p) {
                    // Transition to OPTIMAL 
                    this->state = OPTIMAL;
                }
                else {    
                    // Transition to GREEDY
                    this->state = GREEDY;
                }
                return this->opt_opt->strategy(left, right);
            } else {
                if(d < q) { // Make greedy move
                    // Transition to GREEDY
                    this->state = GREEDY;
                }
                else {       
                    // Transition to OPTIMAL 
                    this->state = OPTIMAL;
                }
                return this->greedy->strategy(left, right);
            }
        }
};

