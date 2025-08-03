#pragma once
#include "./functions.h"
#include "./strategies.h"

using namespace std;

void print(vector<ll> & coins) {
    for(ll t : coins) cout << t << " ";
    cout << endl;
}

int main() {
    srand(time(0));
    int n = 10;
    vector<ll> coins = rand_coins(10, 0, 1000);
    ll gain_opt = gain_under_optimal(coins);
    ll gain_opt_greedy = gain_under_optimal_vs_greedy(coins);
    ll gain_subopt = gain_under_opt_vs_subopt(coins);
    print(coins);
    cout << gain_opt << " " << gain_opt_greedy <<  " " << gain_subopt << endl;
    return 0;
}