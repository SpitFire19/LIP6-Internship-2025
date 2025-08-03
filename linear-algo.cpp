#include "./functions.h"

#include <stack>


using namespace std;

ll linear_algo(vector<ll> coins, int l, int r) {
    int n = r - l + 1;
    stack<ll> coins_stack;

    for(int i = l; i <= r; i++) {
        coins_stack.push(coins[i]);
        while(coins_stack.size() >= 3) {
            ll y = coins_stack.top(); coins_stack.pop();
            ll M = coins_stack.top(); coins_stack.pop();
            ll x = coins_stack.top(); coins_stack.pop();

            if(x <= M && y <= M) {
                coins_stack.push(x - M + y);
            } else {
                coins_stack.push(x);
                coins_stack.push(M);
                coins_stack.push(y);
                break;
            }
        }
    }
    vector<ll> new_coins;
    while(!coins_stack.empty()) {
        new_coins.push_back(coins_stack.top());
        coins_stack.pop();
    }
    reverse(new_coins.begin(), new_coins.end());
    for(auto t : new_coins) {
        cout << t << " ";
    }
    cout << endl;
    int low = 0;
    int up = new_coins.size() - 1;
    ll value = 0;
    int arr[2] = {1, -1};
    int it = 0;
    while(low < up) {
        if(new_coins[l] >= new_coins[up]) {
            value += new_coins[low] * arr[it % 2];
            low++;
        } else {
            value += new_coins[up] * arr[it % 2];
            up--;
        }
        ++it;
    }
    return value - new_coins[low];
}

int main(void) {
    int n = 8;
    vector<ll> coins = {1, 2, 9, 2};
    cout << linear_algo(coins, 0, n - 1);
    return 0;
}