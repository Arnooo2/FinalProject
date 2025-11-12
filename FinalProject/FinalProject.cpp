#include <iostream>
#include <string>

using namespace std;

struct Transaction {
    string type;
    double sum;
    string date;
};

class Account {
private:
    double bal;

public:
    explicit Account(double b = 0.0) {
        bal = b;
    }

    Account(const Account& other) {
        bal = other.bal;
    }

    void add(double s) {
        bal += s;
    }

    void take(double s) {
        if (s <= bal) bal -= s;
        else cout << "Not enough money" << endl;
    }

    double getBal() const {
        return bal;
    }

    void show() const {
        cout << "Balance: " << bal << endl;
    }
};
int main()
{
	//tyt pysto
}

