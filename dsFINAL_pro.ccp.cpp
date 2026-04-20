#include <iostream>
#include <string>
#include <ctime>
#include <queue>
using namespace std;

//  Menu Node 
struct MenuItem {
    int id;
    string name;
    double price;
    string cat;
    int stock;
    MenuItem* left;
    MenuItem* right;

    MenuItem(int i, string n, double p, string c, int s) {
        id = i;
        name = n;
        price = p;
        cat = c;
        stock = s;
        left = right = nullptr;
    }
};

// BST for Menu 
class MenuBST {
private:
    MenuItem* root;

    void insert(MenuItem*& node, int id, string name, double price, string cat, int stock) {
        if (!node) {
            node = new MenuItem(id, name, price, cat, stock);
            return;
        }
        if (name < node->name)
            insert(node->left, id, name, price, cat, stock);
        else
            insert(node->right, id, name, price, cat, stock);
    }

    void display(MenuItem* node, string filter) {
        if (!node) return;
        display(node->left, filter);
        if ((filter == "ALL" || node->cat == filter) && node->stock > 0) {
            cout << "[" << node->id << "] " << node->name
                 << " - Rs. " << node->price
                 << " - Stock: " << node->stock
                 << " - " << node->cat << endl;
        }
        display(node->right, filter);
    }

    MenuItem* search(MenuItem* node, string name) {
        if (!node || node->name == name) return node;
        return (name < node->name) ? search(node->left, name) : search(node->right, name);
    }

    MenuItem* remove(MenuItem* node, string name) {
        if (!node) return nullptr;
        if (name < node->name)
            node->left = remove(node->left, name);
        else if (name > node->name)
            node->right = remove(node->right, name);
        else {
            if (!node->left) {
                MenuItem* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                MenuItem* temp = node->left;
                delete node;
                return temp;
            }
            MenuItem* succ = node->right;
            while (succ->left) succ = succ->left;
            node->name = succ->name;
            node->price = succ->price;
            node->cat = succ->cat;
            node->stock = succ->stock;
            node->right = remove(node->right, succ->name);
        }
        return node;
    }

public:
    MenuBST() { root = nullptr; }

    void add(int id, string name, double price, string cat, int stock) {
        insert(root, id, name, price, cat, stock);
    }

    string getSlot() {
        time_t now = time(0);
        tm* t = localtime(&now);
        int hr = t->tm_hour;
        if (hr >= 6 && hr < 11) return "Breakfast";
        else if (hr >= 11 && hr < 16) return "Lunch";
        else return "Dinner";
    }

    void show(string role) {
        string cat = (role == "admin") ? "ALL" : getSlot();
        cout << "\n--- " << (role == "admin" ? "Full Menu" : cat + " Menu") << " ---\n";
        display(root, cat);
    }

    void find(string name) {
        MenuItem* item = search(root, name);
        if (item) {
            cout << "[" << item->id << "] " << item->name
                 << " - Rs. " << item->price
                 << " - Stock: " << item->stock
                 << " - " << item->cat << endl;
        } else {
            cout << "Item not found.\n";
        }
    }

    void drop(string name) {
        root = remove(root, name);
        cout << name << " removed (if it existed).\n";
    }

    void edit(string name, double price, int stock) {
        MenuItem* item = search(root, name);
        if (item) {
            item->price = price;
            item->stock = stock;
            cout << name << " updated.\n";
        } else {
            cout << "Item not found.\n";
        }
    }

    MenuItem* searchItemInternal(string name) {
        return search(root, name);
    }
};

//Order Queue 
struct Order {
    int id;
    string item;
    int qty;
    string slot;
    time_t time;
};

class OrderQ {
private:
    queue<Order> q;
    int nextID = 1;
    MenuBST* menu;

    string getSlot() {
        time_t now = time(0);
        tm* t = localtime(&now);
        int hr = t->tm_hour;
        if (hr >= 6 && hr < 11) return "Breakfast";
        else if (hr >= 11 && hr < 16) return "Lunch";
        else return "Dinner";
    }

public:
    OrderQ(MenuBST* m) {
	 menu = m; 
	 }

    void place(string item, int qty) {
        MenuItem* mItem = menu->searchItemInternal(item);
        string slot = getSlot();

        if (mItem == NULL) {
            cout << "item not in menu.\n";
            return;
        }

        if (mItem->cat != slot) {
            cout << mItem->name << "only available in" << mItem->cat << ".\n";
            return;
        }

        if (mItem->stock < qty) {
            cout << "only " << mItem->stock << " left.\n";
            return;
        }

        Order o = {nextID++, item, qty, slot, time(0)};
        q.push(o);
        cout << "order placed. ID: " << o.id << endl;
    }

    void serve() {
        if (q.empty()) {
            cout << "No orders.\n";
            return;
        }

        Order o = q.front();
        MenuItem* mItem = menu->searchItemInternal(o.item);
        if (mItem) {
            mItem->stock -= o.qty;
            cout << "Serving Order" << o.id << ": " << o.qty << " x " << o.item << endl;
        }
        q.pop();
    }

    void cancel(int id) {
        if (q.empty()) {
            cout << "No orders to cancel.\n";
            return;
        }

        queue<Order> temp;
        bool found = false;

        size_t size = q.size();
        for (size_t i = 0; i < size; ++i) {
            Order o = q.front(); q.pop();
            if (o.id == id) {
                cout << "order " << id << " cancelled.\n";
                found = true;
            } else {
                temp.push(o);
            }
        }

        q = temp;
        if (!found) cout << "order not found.\n";
    }

    void show() {
        if (q.empty()) {
            cout << "No pending orders.\n";
            return;
        }

        queue<Order> temp = q;
        cout << "--- Orders ---\n";
        while (!temp.empty()) {
            Order o = temp.front(); temp.pop();
            cout << "ID: " << o.id << " | " << o.qty << " x " << o.item << " | " << o.slot << endl;
        }
    }
};

// Main
int main() {
    MenuBST menu;
    menu.add(101, "Paratha", 50, "Breakfast", 20);
    menu.add(201, "Biryani", 220, "Lunch", 30);
    menu.add(301, "Pizza", 600, "Dinner", 12);

    OrderQ orderSys(&menu);

    string role;
    cout << "Login as (admin/customer): ";
    cin >> role;

    if (role == "admin") {
        string pwd;
        cout << "Enter admin password: ";
        cin >> pwd;
        if (pwd != "admin123") {
            cout << "Wrong password.\n";
            return 0;
        }
    }

    int ch;
    do {
        cout << "\n--- " << role << " MENU ---\n";
        cout << "1. Show Menu\n";
        cout << "2. Search Item\n";

        if (role == "admin") {
            cout << "3. Add Item\n4. Delete Item\n5. Update Item\n";
        } else {
            cout << "3. Place Order\n4. Serve Order\n5. Cancel Order\n6. Show Orders\n";
        }

        cout << "0. Exit\nEnter choice: ";
        cin >> ch;
        cin.ignore(); 

        if (ch == 1) menu.show(role);
        else if (ch == 2) {
            string n;
            cout << "Item name: "; getline(cin, n);
            menu.find(n);
        }
        else if (role == "admin" && ch == 3) {
            int id, s; double p; string n, c;
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin.ignore(); getline(cin, n);
            cout << "Price: "; cin >> p;
            cout << "Category: "; cin >> c;
            cout << "Stock: "; cin >> s;
            menu.add(id, n, p, c, s);
        }
        else if (role == "admin" && ch == 4) {
            string n;
            cout << "Name to delete: "; getline(cin, n);
            menu.drop(n);
        }
        else if (role == "admin" && ch == 5) {
            string n; double p; int s;
            cout << "Name: "; getline(cin, n);
            cout << "New price: "; cin >> p;
            cout << "New stock: "; cin >> s;
            menu.edit(n, p, s);
        }
        else if (role != "admin" && ch == 3) {
            string n; int qty;
            cout << "Item: "; getline(cin, n);
            cout << "Qty: "; cin >> qty;
            orderSys.place(n, qty);
        }
        else if (role != "admin" && ch == 4) {
            orderSys.serve();
        }
        else if (role != "admin" && ch == 5) {
            int oid;
            cout << "Order ID to cancel: ";
            cin >> oid;
            orderSys.cancel(oid);
        }
        else if (role != "admin" && ch == 6) {
            orderSys.show();
        }
        else if (ch == 0) {
            cout << "Goodbye!\n";
        }
        else {
            cout << "Invalid choice.\n";
        }

    } while (ch != 0);

    return 0;
}
