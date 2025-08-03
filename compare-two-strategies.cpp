#include "./functions.h"

double greedy_opt_ratio(vector<ll> coins)
{
    int n = coins.size();
    matrix<bool> greedy = greedy_play(coins, 0, n - 1);
    matrix<bool> optimal = optimal_play(coins, 0, n - 1);
    ll total = n * (n + 2)  / 4;
    ll count = 0;

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
    cout << "Mean ratio where greedy coincides with optimal is " << total_mean << endl;
}