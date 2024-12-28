#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds {};

class Account {
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);
    long getAccNo() { return accountNumber; }
    string getFirstName() { return firstName;  }
    string getLastName() { return lastName; }
    float getBalance() { return balance; }
    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();
    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};

long Account::NextAccountNumber = 0;

class Bank {
private:
    map<long, Account> accounts;

public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
    ~Bank();
};

int main() {
    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accountNumber;
    float balance, amount;

    cout << "Banking System" << endl;
    do {
        cout << "\n1. Open Account\n2. Balance Enquiry\n3. Deposit\n4. Withdraw\n5. Close Account\n6. Show All Accounts\n7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            cout << "Enter Initial Balance: ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance);
            cout << "Account Created Successfully!" << endl;
            cout << acc;
            break;
        case 2:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            try {
                acc = b.BalanceEnquiry(accountNumber);
                cout << "Account Details:\n" << acc;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 3:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Amount to Deposit: ";
            cin >> amount;
            try {
                acc = b.Deposit(accountNumber, amount);
                cout << "Amount Deposited Successfully!" << endl;
                cout << acc;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 4:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;
            try {
                acc = b.Withdraw(accountNumber, amount);
                cout << "Amount Withdrawn Successfully!" << endl;
                cout << acc;
            } catch (InsufficientFunds &) {
                cout << "Error: Insufficient Funds!" << endl;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 5:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            try {
                b.CloseAccount(accountNumber);
                cout << "Account Closed Successfully!" << endl;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 6:
            b.ShowAllAccounts();
            break;
        case 7:
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}


Account::Account(string fname, string lname, float balance) {
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}

void Account::Deposit(float amount) {
    balance += amount;
}

void Account::Withdraw(float amount) {
    if (balance - amount < MIN_BALANCE) {
        throw InsufficientFunds();
    }
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber) {
    NextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber() {
    return NextAccountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc) {
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, Account &acc) {
    os << "Account Number: " << acc.getAccNo() << endl;
    os << "First Name: " << acc.getFirstName() << endl;
    os << "Last Name: " << acc.getLastName() << endl;
    os << "Balance: " << acc.getBalance() << endl;
    return os;
}

// Bank Class Definitions
Bank::Bank() {
    ifstream infile("Bank.data");
    if (!infile) {
        return;
    }
    Account account;
    while (infile >> account) {
        accounts[account.getAccNo()] = account;
    }
    infile.close();
    if (!accounts.empty()) {
        Account::setLastAccountNumber((--accounts.end())->first);
    }
}

Account Bank::OpenAccount(string fname, string lname, float balance) {
    Account account(fname, lname, balance);
    accounts[account.getAccNo()] = account;
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) {
        throw runtime_error("Account number does not exist!");
    }
    return itr->second;
}

Account Bank::Deposit(long accountNumber, float amount) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) {
        throw runtime_error("Account number does not exist!");
    }
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::Withdraw(long accountNumber, float amount) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) {
        throw runtime_error("Account number does not exist!");
    }
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::CloseAccount(long accountNumber) {
    auto itr = accounts.find(accountNumber);
    if (itr == accounts.end()) {
        throw runtime_error("Account number does not exist!");
    }
    accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts() {
    for (auto &acc : accounts) {
        cout << acc.second;
    }
}

Bank::~Bank() {
    ofstream outfile("Bank.data", ios::trunc);
    for (auto &acc : accounts) {
        outfile << acc.second;
    }
    outfile.close();
}