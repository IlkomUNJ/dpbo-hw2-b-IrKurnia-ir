#include "bank_customer.h"

void BankCustomer::deposit(double amount) {
    balance += amount;
    transactionHistory.push_back("Deposit: $" + to_string(amount));
    cout << "Deposited $" << amount << ". New balance: $" << balance << endl;
}

void BankCustomer::withdraw(double amount) {
    if (amount > balance) {
        cout << "Withdrawal rejected: Insufficient funds." << endl;
        return;
    }
    balance -= amount;
    transactionHistory.push_back("Withdraw: $" + to_string(amount));
    cout << "Withdrawn $" << amount << ". New balance: $" << balance << endl;
}

void BankCustomer::printTransactionHistory() const {
    cout << "\n=== Transaction History ===" << endl;
    if (transactionHistory.empty()) {
        cout << "No transactions recorded." << endl;
        return;
    }
    for (const auto& t : transactionHistory)
        cout << "- " << t << endl;
}
