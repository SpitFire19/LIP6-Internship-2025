#include "./functions.h"

int main()
{
    srand(time(NULL));
    int n = 16;
    int A = 0, B = 1000;
    double total_mean = 0.0;

    vector<ll> lst = {0, 1};

    vector<ll> coins_initial = rand_coins(n, A, B);
    double min_mean = greedy_opt_ratio(coins_initial);

    vector<ll> min_coins(n), max_coins(n);
    for (;;)
    {
        vector<ll> coins = rand_coins_from_list(n, lst);
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
