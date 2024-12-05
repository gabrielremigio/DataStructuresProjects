#include<iostream>
#include<string>
#include <cstdint>

using namespace std;

int64_t kFatorial(int n, int k)
{
    int64_t result = n;
    int temp = k;
    while (n - temp >= 1) {
        result *= (n - temp);
        temp += k;
    }
    return result;
}

int main()
{
    int numberOfEntries{};
    cin >> numberOfEntries;


    string line{};
    int pos{};
    int num1;
    int num2;

    for (int i = 0; i < numberOfEntries; i++) {
        cin >> line;
        pos = line.find('!');
        num1 = stoi(line);
        num2 = line.substr(pos).length();

        cout << kFatorial(num1, num2) << endl;
    }
    return 0;
}
