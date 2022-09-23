#include <vector>
#include <iostream>

using namespace std;

int main(){
    vector<vector<int>> v;
    vector<int> v1 = {1, 2, 4, 5, 3};
    vector<int> v2 = {2, 3, 5, 3, 2};
    vector<int> v3 = {2, 3, 5, 3, 2};
    v.push_back(v1);
    v.push_back(v2);
    v.push_back(v3);
    for (vector<int> list : v){
        for (int i : list)
            cout << i << "\t";
        cout << endl;
    }
    return 0;
}