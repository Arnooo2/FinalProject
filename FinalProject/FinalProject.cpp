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

    double getBal() const {
        return bal;
    }

    void show() const {
        cout << "Balance: " << bal << endl;
    }
    Account& operator+=(double s) {
        bal += s;
        return *this;
    }

    Account& operator-=(double s) {
        if (s <= bal) bal -= s;
        else cout << "Not enough money" << endl;
        return *this;
    }

    Account operator+(double s) const {
        Account temp(*this);
        temp.bal += s;
        return temp;
    }

    Account operator-(double s) const {
        Account temp(*this);
        if (s <= temp.bal) temp.bal -= s;
        else cout << "Not enough money" << endl;
        return temp;
    }
};
int main()
{
    Account acc(100); 
    acc.show();

    acc += 20;
    acc.show();

    acc -= 10;
    acc.show();

    Account acc2 = acc + 40;
	acc2.show();

    Account acc3 = acc - 15;
    acc3.show();


}

