#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include <vector>
#include "cart_item.h"
#include <iostream>
using namespace std;

enum InvoiceStatus { UNPAID, PAID };

class Invoice {
private:
    int invoiceId;
    string buyerName;
    string sellerName;
    vector<CartItem> items;
    double totalAmount;
    InvoiceStatus status;

public:
    Invoice(int id, const string& buyer, const string& seller, const vector<CartItem>& cartItems)
        : invoiceId(id), buyerName(buyer), sellerName(seller), items(cartItems), status(UNPAID) {
        totalAmount = 0;
        for (const auto& c : items)
            totalAmount += c.price * c.quantity;
    }

    int getId() const { return invoiceId; }
    double getTotal() const { return totalAmount; }
    string getBuyer() const { return buyerName; }
    string getSeller() const { return sellerName; }
    InvoiceStatus getStatus() const { return status; }
    void markPaid() { status = PAID; }

    void printDetails() const {
        cout << "\n=== Invoice #" << invoiceId << " ===\n";
        cout << "Buyer: " << buyerName << "\nSeller: " << sellerName << endl;
        for (const auto& i : items)
            cout << " - " << i.itemName << " x" << i.quantity << " @ $" << i.price << endl;
        cout << "Total: $" << totalAmount << "\nStatus: "
             << (status == PAID ? "PAID" : "UNPAID") << endl;
    }
};
#endif
