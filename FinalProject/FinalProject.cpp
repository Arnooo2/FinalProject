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

    virtual void show() const {
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
        Account a(*this);
        a.bal += s;
        return a;
    }

    Account operator-(double s) const {
        Account a(*this);
        if (s <= a.bal) a.bal -= s;
        else cout << "Not enough money" << endl;
        return a;
    }
    Account& operator=(const Account& other) {
        if (this != &other) {
            bal = other.bal;
        }
        return *this;
	}
};

class  CreditAccount : public Account {
private:
    int limit;
public:
    explicit CreditAccount(double b = 0.0, int l = 0) : Account(b), limit(l) {};
    Account& operator-=(double s) {
        if (s <= getBal() + limit) {
            double nb = getBal() - s;
            Account a(nb);
            Account::operator=(a);
        }
        else cout << "Not enough money" << endl;
        return *this;
    }
};

void menu(Account& acc, CreditAccount& crAcc) {
    int choice;
    double sum;

    while (true) {
        cout << "\n=====~~ONLINE~BANK~~=====\n";
        cout << "1. Show balances\n";
        cout << "2. Deposit to Account\n";
        cout << "3. Withdraw from Account\n";
        cout << "4. Deposit to CreditAccount\n";
        cout << "5. Withdraw from CreditAccount\n";
        cout << "6. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "\n-=- Regular Account -=-\n";
            acc.show();

            cout << "\n-=- Credit Account -=-\n";
            crAcc.show();
            break;

        case 2:
            cout << "Enter amount to deposit: ";
            cin >> sum;
            acc += sum;
            break;

        case 3:
            cout << "Enter amount to withdraw: ";
            cin >> sum;
            acc -= sum;
            break;

        case 4:
            cout << "Enter amount to deposit: ";
            cin >> sum;
            crAcc += sum;
            break;

        case 5:
            cout << "Enter amount to withdraw: ";
            cin >> sum;
            crAcc -= sum;
            break;

        case 6:
            return;
        default:
            cout << "Error!" << endl;
        }
    }
}
int main()
{
    /*CreditAccount cacc(100, 200);

    cacc.show();

    cacc -= 50;
    cacc.show();

    cacc -= 200;
    cacc.show(); 

    cacc -= 200;   
    cacc.show();*/   

	Account acc(500);
	CreditAccount crAcc(500, 300);
	menu(acc, crAcc);
}

