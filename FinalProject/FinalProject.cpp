#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <mutex>   

using namespace std;

struct Transaction {
    string type;
    double sum;
};
vector<Transaction> transactions;


mutex transactionsMutex;

class BankException : public std::exception {
private:
    string msg;
public:
    explicit BankException(const string& m) : msg(m) {}
    const char* what() const noexcept override {
        return msg.c_str();
    }
};


void addTransaction(const string& type, double sum) {
    lock_guard<mutex> lock(transactionsMutex);
    Transaction t{ type, sum };
    transactions.push_back(t);
}

void saveTransactionsToFile(vector<Transaction> t) {
    lock_guard<mutex> lock(transactionsMutex);
    ofstream file("History.txt", ios::app);
    file << "=== TRANSACTION HISTORY ===\n";
    for (int i = 0; i < t.size(); i++) {
        file << t[i].type << ": " << t[i].sum << "\n";
    }
    file << "===========================\n\n";
    file.close();
    cout << "History saved to file.\n";
}

void printTransactionsTable(const vector<Transaction>& t)
{
    lock_guard<mutex> lock(transactionsMutex);
    cout << "\n================ TRANSACTION TABLE ================\n";
    cout << "|     TYPE        |        SUM                    |\n";
    cout << "---------------------------------------------------\n";

    for (int i = 0; i < t.size(); i++)
    {
        cout << "| " << t[i].type;

        int spaces = 15 - t[i].type.size();
        for (int s = 0; s < spaces; s++) cout << " ";

        cout << "| " << t[i].sum << "\n";
    }

    cout << "===================================================\n\n";
}



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
        addTransaction("Deposit", s);
        return *this;
    }

    Account& operator-=(double s) {
        if (s <= bal) {
            bal -= s;
            addTransaction("Withdraw", s);
        }
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
        try {
            cout << "\n=====~~ONLINE~BANK~~=====\n";
            cout << "1. Show balances\n";
            cout << "2. Deposit to Account\n";
            cout << "3. Withdraw from Account\n";
            cout << "4. Deposit to CreditAccount\n";
            cout << "5. Withdraw from CreditAccount\n";
            cout << "6. Exit\n";
            cout << "7. Show transaction table\n";
            cout << "Choose: ";

            if (!(cin >> choice))
                throw BankException("Invalid menu input!");

            switch (choice) {
            case 7:
                printTransactionsTable(transactions);
                break;

            case 1:
                acc.show();
                crAcc.show();
                break;

            case 2:
                cout << "Enter amount to deposit: ";
                if (!(cin >> sum) || sum <= 0)
                    throw BankException("Incorrect deposit value!");
                acc += sum;
                break;

            case 3:
                cout << "Enter amount to withdraw: ";
                if (!(cin >> sum) || sum <= 0)
                    throw BankException("Incorrect withdraw value!");
                if (sum > acc.getBal())
                    throw BankException("Not enough money on Account!");
                acc -= sum;
                break;

            case 4:
                cout << "Enter amount to deposit: ";
                if (!(cin >> sum) || sum <= 0)
                    throw BankException("Incorrect deposit value!");
                crAcc += sum;
                break;

            case 5:
                cout << "Enter amount to withdraw: ";
                if (!(cin >> sum) || sum <= 0)
                    throw BankException("Incorrect withdraw value!");
                if (sum > crAcc.getBal() + 300)
                    throw BankException("Credit limit exceeded!");
                crAcc -= sum;
                break;

            case 6:
                saveTransactionsToFile(transactions);
                return;

            default:
                throw BankException("Invalid menu option!");
            }
        }
        catch (BankException& e) {
            cout << "[ERROR] " << e.what() << "\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

void printTransactionsTable(const vector<Transaction>& t);


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
    CreditAccount cacc(500, 300);
    menu(acc, cacc);
}