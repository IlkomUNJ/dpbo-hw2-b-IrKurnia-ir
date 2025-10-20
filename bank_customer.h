#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class BankCustomer {
private:
    int id;
    string name;
    double balance;
    vector<string> transactionHistory;

public:
    BankCustomer(int id, const string& name, double balance)
        : id(id), name(name), balance(balance) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getBalance() const { return balance; }

    void setBalance(double amount) { balance = amount; }

    void deposit(double amount);
    void withdraw(double amount);
    void printTransactionHistory() const;
};

#endif
