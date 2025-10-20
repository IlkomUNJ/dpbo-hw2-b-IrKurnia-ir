#ifndef CART_ITEM_H
#define CART_ITEM_H
#include <string>
using namespace std;

class CartItem {
public:
    int itemId;
    string itemName;
    int quantity;
    double price;
    string sellerName;

    CartItem(int id, string name, int qty, double prc, string seller)
        : itemId(id), itemName(name), quantity(qty), price(prc), sellerName(seller) {}
};
#endif
