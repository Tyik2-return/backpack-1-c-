#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class backpack {
    int maxPrice = 0;
    int maxState = 0;

    int len;
    int weight;
    vector<vector<int>> things; ; // 0 строка цена 1 строка вес
    vector<int> weight_things_in_backpack, prices_things_in_backpack;
public:

    backpack(int length, int weight_backpack) {
        len = length;
        weight = weight_backpack;
        things = vector<vector<int>>(2, vector<int>(len));
    }

    backpack(vector<int>& weights, vector<int>& prices, int weight_backpack, int length) {
        len = length;
        weight = weight_backpack;
        things = vector<vector<int>>(2, vector<int>(len));
        for (int i = 0; i < len; i++) {
            things[1][i] = weights[i];
            things[0][i] = prices[i];
        }
    }
    void MainGreedy() { 
        vector<pair<double, int>> value_per_weight;
        for (int i = 0; i < len; i++) {
            value_per_weight.push_back({ (double)things[0][i] / things[1][i], i });
        }
         sort(value_per_weight.rbegin(), value_per_weight.rend());
        int sum_weight = 0, sum_price = 0, sum_things = 0;
        for (int i = 0; i < len; i++) {
            if (sum_weight + things[1][value_per_weight[i].second] <= weight) {
                sum_weight += things[1][value_per_weight[i].second];
                sum_price += things[0][value_per_weight[i].second];
                sum_things++;
                weight_things_in_backpack.push_back(things[1][value_per_weight[i].second]);
                prices_things_in_backpack.push_back(things[0][value_per_weight[i].second]);
            }
        }
        maxState = sum_things;
        maxPrice = sum_price;
    }
    void cout_backpack() {
        cout << "максимальная сумма в рюкзаке: " << maxPrice << endl;
        cout << "всего вещей: " << len << endl;
        cout << "кол-во вещей в сумке: " << maxState << endl;
        cout << "цена - вес предмета: " << endl;
        for (int j = 0; j < weight_things_in_backpack.size(); j++) {
            cout << prices_things_in_backpack[j] << " - " << weight_things_in_backpack[j] << endl;
        }
    }
    void MainDinamProg() {
        vector<vector<int>> dp = vector<vector<int>>(weight + 1, vector<int>(len + 1));
        for (int i = 0; i <= weight; i++) {
            dp[i][0] = 0;
        }
        for (int j = 0; j <= len; j++) {
            dp[0][j] = 0;
        }

        for (int i = 1; i <= weight; i++) {
            for (int j = 1; j <= len; j++) {
                dp[i][j] = dp[i][j - 1];
                if (i >= things[1][j - 1] && dp[i][j] < dp[i - things[1][j - 1]][j - 1] + things[0][j - 1]) {
                    dp[i][j] = dp[i - things[1][j - 1]][j - 1] + things[0][j - 1];
                }
            }
        }

        
        maxPrice = dp[weight][len];
        int target_w = weight, target_l = len;
        while (target_w > 0 && target_l > 0) {
            if (dp[target_w][target_l] != dp[target_w][target_l - 1]) {
                weight_things_in_backpack.push_back(things[1][target_l - 1]);
                prices_things_in_backpack.push_back(things[0][target_l - 1]);
                maxState++;
                target_w -= things[1][target_l - 1];

            }
            target_l--;
        }
    }
    void MainBruiteForce() {
        for (int i = 0; i < pow(2, len); i++) {
            int sum_weight = 0, sum_price = 0, sum_things = 0;

            for (int j = 0; j < len; j++) {
                if (i & (1 << j)) 
                {
                    
                    sum_weight += things[1][j];
                    sum_price += things[0][j];
                    sum_things++;
                }
            }

            if (sum_weight <= weight && sum_price > maxPrice) {
                maxPrice = sum_price;
                maxState = sum_things;
                weight_things_in_backpack.clear();
                prices_things_in_backpack.clear();

                for (int j = 0; j < len; j++) {
                   if (i & (1 << j)) 
                    {
                        weight_things_in_backpack.push_back(things[1][j]);
                        prices_things_in_backpack.push_back(things[0][j]);
                    }
                }
            }
        }
    }

};

int main() {
    setlocale(LC_ALL, "RU");
    int n, weight_backpack;
    cout << "введите кол-во вещй и вес рюкзака:";
    cin >> n >> weight_backpack;
    vector<int> weights(n), prices(n);
    for (int i = 0; i < n; i++) {
        weights[i] = rand() % 10 + 4;

        prices[i] = rand() % 7 + 4 + (rand() % 7 - 3);
       //cout << prices[i] << " - " << weights[i] << endl;
    }

    cout << endl;
   backpack bag2(weights, prices, weight_backpack, n);
   bag2.MainDinamProg();
   bag2.cout_backpack();

   backpack bag3(weights, prices, weight_backpack, n);
   bag3.MainGreedy();
   bag3.cout_backpack();
    if (n<=30)
    {
        backpack bag(weights, prices, weight_backpack, n);
        bag.MainBruiteForce();
        bag.cout_backpack();
    }
    return 0;
}