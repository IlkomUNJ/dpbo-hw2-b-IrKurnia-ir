#ifndef ITEM_H
#define ITEM_H
#include <string>
using namespace std;

class Item {
private:
    int id;
    string name;
    int quantity;
    double price;
    bool idDisplay;

public:
    Item(int id, const string& name, int quantity, double price)
        : id(id), name(name), quantity(quantity), price(price), idDisplay(false) {}

    int getId() const { return id; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }
    void setDisplay(bool display) { idDisplay = display; }
};
#endif
