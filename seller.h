#ifndef SELLER_H
#define SELLER_H

#include "buyer.h"
#include "item.h"
#include <vector>
#include <string>
using namespace std;

class seller : public Buyer {
public:
    int sellerId;
    string sellerName;
    vector<Item> items;

    seller() = default;

    seller(Buyer buyer, int sellerId, const string& sellerName)
        : Buyer(buyer.getId(), buyer.getName(), buyer.getAccount()),
          sellerId(sellerId), sellerName(sellerName) {}

    void addNewItem(int id, const string& name, int qty, double price) {
        Item newItem(id, name, qty, price);
        items.push_back(newItem);
    }
};
#endif
