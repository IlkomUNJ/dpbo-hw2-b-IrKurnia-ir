#include <iostream>
#include <vector>
#include <algorithm>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "item.h"
#include "cart_item.h"
#include "invoice.h"

using namespace std;

enum PrimaryPrompt { LOGIN, REGISTER, EXIT };

int main() {
    vector<Buyer> buyers;
    vector<seller> sellers;
    vector<CartItem> cart;
    vector<Invoice> invoices;

    PrimaryPrompt prompt = LOGIN;

    while (prompt != EXIT) {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Select option: ";
        int choice;
        cin >> choice;
        prompt = static_cast<PrimaryPrompt>(choice - 1);

        switch (prompt) {

            // ================= REGISTER ==================
            case REGISTER: {
                cout << "\n=== Register New Buyer Account ===\n";
                int id;
                string name;
                double deposit;

                cout << "Enter your desired Buyer ID (numbers only): ";
                cin >> id;
                cin.ignore();
                cout << "Enter your Name: ";
                getline(cin, name);
                cout << "Enter Initial Deposit: ";
                cin >> deposit;

                auto* newAccount = new BankCustomer(id, name, deposit);
                Buyer newBuyer(id, name, *newAccount);
                buyers.push_back(newBuyer);

                cout << "\n Buyer account created successfully!\n";
                cout << "Your Buyer ID: " << id << endl;
                cout << "Initial Balance: $" << deposit << endl;
                cout << "You can now login using your Buyer ID.\n";
                break;
            }

            // ================= LOGIN ==================
            case LOGIN: {
                cout << "\n=== LOGIN ===\nEnter Buyer ID: ";
                int buyerId;
                cin >> buyerId;

                Buyer* loggedInBuyer = nullptr;
                for (auto& b : buyers)
                    if (b.getId() == buyerId) loggedInBuyer = &b;

                if (!loggedInBuyer) {
                    cout << " Buyer not found. Please register first.\n";
                    break;
                }

                seller* linkedSeller = nullptr;
                for (auto& s : sellers)
                    if (s.getId() == buyerId) linkedSeller = &s;

                bool buyerMenu = true;
                while (buyerMenu) {
                    cout << "\n--- Buyer Menu ---\n";
                    cout << "1. Check Account Status\n";
                    if (!linkedSeller)
                        cout << "2. Upgrade Account to Seller\n";
                    cout << "3. Banking Functions\n";
                    cout << "4. Browse Store\n";
                    cout << "5. View Cart & Checkout\n";
                    cout << "6. Payment Functionality\n";
                    if (linkedSeller)
                        cout << "7. Seller Functions\n";
                    cout << "8. Logout\n";
                    cout << "9. Delete Account\n";
                    cout << "Select: ";
                    int buyerChoice;
                    cin >> buyerChoice;

                    switch (buyerChoice) {

                        // 1 CHECK STATUS
                        case 1: {
                            cout << "\n=== Account Status ===\n";
                            cout << "Name: " << loggedInBuyer->getName() << endl;
                            cout << "Balance: $" << loggedInBuyer->getAccount().getBalance() << endl;
                            cout << "Account Type: " << (linkedSeller ? "Buyer & Seller" : "Buyer Only") << endl;
                            break;
                        }

                        // 2 UPGRADE TO SELLER (hanya muncul jika belum seller)
                        case 2: {
                            if (linkedSeller) {
                                cout << "Already a seller.\n";
                                break;
                            }

                            cout << "\n=== Upgrade Account to Seller ===\n";
                            if (loggedInBuyer->getAccount().getBalance() <= 0) {
                                cout << "Upgrade rejected: You must have a bank account with balance.\n";
                                break;
                            }

                            string storeName;
                            cout << "Enter Store Name: ";
                            cin.ignore();
                            getline(cin, storeName);

                            sellers.emplace_back(*loggedInBuyer, sellers.size() + 1, storeName);
                            linkedSeller = &sellers.back();

                            cout << " Successfully upgraded to Seller.\n";
                            cout << "Welcome to Seller Mode (" << storeName << ")!\n";
                            break;
                        }

                        // 3 BANKING FUNCTIONS
                        case 3: {
                            bool bankMenu = true;
                            while (bankMenu) {
                                cout << "\n=== Banking Menu ===\n";
                                cout << "1. Check Balance\n";
                                cout << "2. Deposit\n";
                                cout << "3. Withdraw\n";
                                cout << "4. Back\n";
                                cout << "Select: ";
                                int bankChoice;
                                cin >> bankChoice;

                                switch (bankChoice) {
                                    case 1:
                                        cout << "Balance: $" << loggedInBuyer->getAccount().getBalance() << endl;
                                        break;
                                    case 2: {
                                        double amt;
                                        cout << "Deposit amount: ";
                                        cin >> amt;
                                        loggedInBuyer->getAccount().deposit(amt);
                                        break;
                                    }
                                    case 3: {
                                        double amt;
                                        cout << "Withdraw amount: ";
                                        cin >> amt;
                                        loggedInBuyer->getAccount().withdraw(amt);
                                        break;
                                    }
                                    case 4:
                                        bankMenu = false;
                                        break;
                                }
                            }
                            break;
                        }

                        // 4 BROWSE STORE
                        case 4: {
                            if (sellers.empty()) {
                                cout << "No stores yet.\n";
                                break;
                            }
                            cout << "\n=== Browse Stores ===\n";
                            for (size_t i = 0; i < sellers.size(); ++i)
                                cout << i + 1 << ". " << sellers[i].sellerName << endl;

                            cout << "Select store: ";
                            int storeChoice;
                            cin >> storeChoice;
                            if (storeChoice <= 0 || storeChoice > (int)sellers.size()) break;
                            seller& sel = sellers[storeChoice - 1];

                            if (sel.items.empty()) {
                                cout << "Store has no items yet.\n";
                                break;
                            }

                            cout << "\nItems in " << sel.sellerName << ":\n";
                            for (auto& i : sel.items)
                                cout << i.getId() << ". " << i.getName()
                                     << " - $" << i.getPrice()
                                     << " (" << i.getQuantity() << " left)\n";

                            cout << "Select Item ID to add to cart: ";
                            int itemChoice;
                            cin >> itemChoice;
                            for (auto& i : sel.items)
                                if (i.getId() == itemChoice) {
                                    cart.push_back(CartItem(i.getId(), i.getName(), 1, i.getPrice(), sel.sellerName));
                                    cout << "Added " << i.getName() << " to cart.\n";
                                }
                            break;
                        }

                        // 5 CART & CHECKOUT
                        case 5: {
                            bool orderMenu = true;
                            while (orderMenu) {
                                cout << "\n=== Cart Menu ===\n";
                                cout << "1. View Cart\n";
                                cout << "2. Remove Item\n";
                                cout << "3. Checkout\n";
                                cout << "4. Back\n";
                                cout << "Select: ";
                                int c;
                                cin >> c;

                                switch (c) {
                                    case 1: {
                                        if (cart.empty()) {
                                            cout << "Cart is empty.\n";
                                            break;
                                        }
                                        double total = 0;
                                        for (size_t i = 0; i < cart.size(); ++i) {
                                            cout << i + 1 << ". " << cart[i].itemName
                                                 << " x" << cart[i].quantity
                                                 << " $" << cart[i].price << endl;
                                            total += cart[i].price;
                                        }
                                        cout << "Total: $" << total << endl;
                                        break;
                                    }
                                    case 2: {
                                        int idx;
                                        cout << "Enter index to remove: ";
                                        cin >> idx;
                                        if (idx > 0 && idx <= (int)cart.size())
                                            cart.erase(cart.begin() + idx - 1);
                                        break;
                                    }
                                    case 3: {
                                        if (cart.empty()) { cout << "Cart empty.\n"; break; }
                                        string sellerName = cart.front().sellerName;
                                        invoices.push_back(Invoice(invoices.size() + 1, loggedInBuyer->getName(), sellerName, cart));
                                        cart.clear();
                                        cout << "Checkout done. Invoice created.\n";
                                        break;
                                    }
                                    case 4: orderMenu = false; break;
                                }
                            }
                            break;
                        }

                        // 6 PAYMENT
                        case 6: {
                            if (invoices.empty()) { cout << "No invoices.\n"; break; }
                            cout << "\n=== Invoices ===\n";
                            for (auto& inv : invoices)
                                cout << "#" << inv.getId() << " | " << inv.getSeller()
                                     << " | $" << inv.getTotal()
                                     << " | " << (inv.getStatus() == PAID ? "PAID" : "UNPAID") << endl;
                            cout << "Enter Invoice ID to pay: ";
                            int id; cin >> id;
                            for (auto& inv : invoices) {
                                if (inv.getId() == id && inv.getBuyer() == loggedInBuyer->getName()) {
                                    inv.printDetails();
                                    if (inv.getStatus() == PAID) { cout << "Already paid.\n"; break; }
                                    double bal = loggedInBuyer->getAccount().getBalance();
                                    if (bal < inv.getTotal()) { cout << "Insufficient funds.\n"; break; }
                                    cout << "Confirm payment (enter invoice ID again): ";
                                    int conf; cin >> conf;
                                    if (conf == id) {
                                        loggedInBuyer->getAccount().withdraw(inv.getTotal());
                                        inv.markPaid();
                                        cout << "Payment successful!\n";
                                    } else cout << "Cancelled.\n";
                                    break;
                                }
                            }
                            break;
                        }

                        // 7 SELLER FUNCTIONS 
                        case 7: {
                            if (!linkedSeller) {
                                cout << "You are not a seller yet.\n";
                                break;
                            }

                            bool sellerMenu = true;
                            while (sellerMenu) {
                                cout << "\n--- Seller Menu ---\n";
                                cout << "1. Check Inventory\n";
                                cout << "2. Add Item\n";
                                cout << "3. Remove Item\n";
                                cout << "4. View Orders (Paid Only)\n";
                                cout << "5. Back\n";
                                cout << "Select: ";
                                int sellerChoice;
                                cin >> sellerChoice;

                                switch (sellerChoice) {
                                    case 1: {
                                        cout << "\n=== Inventory ===\n";
                                        if (linkedSeller->items.empty())
                                            cout << "No items yet.\n";
                                        else {
                                            for (const auto& item : linkedSeller->items)
                                                cout << "ID: " << item.getId()
                                                     << " | " << item.getName()
                                                     << " | Qty: " << item.getQuantity()
                                                     << " | $" << item.getPrice() << endl;
                                        }
                                        break;
                                    }
                                    case 2: {
                                        int id, qty;
                                        double price;
                                        string name;
                                        cout << "Enter Item ID: ";
                                        cin >> id;
                                        cin.ignore();
                                        cout << "Enter Item Name: ";
                                        getline(cin, name);
                                        cout << "Enter Quantity: ";
                                        cin >> qty;
                                        cout << "Enter Price: ";
                                        cin >> price;
                                        linkedSeller->addNewItem(id, name, qty, price);
                                        cout << "Item added.\n";
                                        break;
                                    }
                                    case 3: {
                                        int id;
                                        cout << "Enter Item ID to remove: ";
                                        cin >> id;
                                        auto& inv = linkedSeller->items;
                                        inv.erase(remove_if(inv.begin(), inv.end(),
                                            [id](const Item& i){ return i.getId() == id; }), inv.end());
                                        cout << "Item removed (if existed).\n";
                                        break;
                                    }
                                    case 4: {
                                        cout << "\n=== Orders (Paid Only) ===\n";
                                        bool found = false;
                                        for (const auto& inv : invoices) {
                                            if (inv.getSeller() == linkedSeller->sellerName && inv.getStatus() == PAID) {
                                                inv.printDetails();
                                                found = true;
                                            }
                                        }
                                        if (!found) cout << "No paid orders yet.\n";
                                        break;
                                    }
                                    case 5:
                                        sellerMenu = false;
                                        break;
                                }
                            }
                            break;
                        }

                        // 8 LOGOUT
                        case 8: {
                            char confirm;
                            cout << "Are you sure you want to logout? (y/n): ";
                            cin >> confirm;
                            if (confirm == 'y' || confirm == 'Y') buyerMenu = false;
                            break;
                        }

                        // 9 DELETE ACCOUNT
                        case 9: {
                            char confirm;
                            cout << "Delete your account? (y/n): ";
                            cin >> confirm;
                            if (confirm == 'y' || confirm == 'Y') {
                                buyers.erase(remove_if(buyers.begin(), buyers.end(),
                                    [&](Buyer& b) { return b.getId() == loggedInBuyer->getId(); }), buyers.end());
                                sellers.erase(remove_if(sellers.begin(), sellers.end(),
                                    [&](seller& s) { return s.getId() == loggedInBuyer->getId(); }), sellers.end());
                                cout << "Account deleted.\n";
                                buyerMenu = false;
                            }
                            break;
                        }
                    }
                }
                break;
            }

            // ================= EXIT ==================
            case EXIT:
                cout << "STOP!\n";
                break;
        }
    }
}
