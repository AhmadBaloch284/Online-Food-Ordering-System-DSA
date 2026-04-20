#include <iostream>
#include <queue>
#include <string>
using namespace std;

struct MenuItem {
    int foodId;
    string name;
    int price;
    int quantity;
    MenuItem* left;
    MenuItem* right;

    MenuItem(int id, string n, int p, int q) {
        foodId = id;
        name = n;
        price = p;
        quantity = q;
        left = right = nullptr;
    }
};

struct Order {
    int orderId;
    string customerName;
    int foodId;
    int quantity;
    string orderDate;

    Order(int id, string name, int fId, int qty, string date) {
        orderId = id;
        customerName = name;
        foodId = fId;
        quantity = qty;
        orderDate = date;
    }
};

queue<Order> orders;

MenuItem* insertMenuItem(MenuItem* root, int id, string name, int price, int quantity) {
    if (root == nullptr)
        return new MenuItem(id, name, price, quantity);
    if (id < root->foodId)
        root->left = insertMenuItem(root->left, id, name, price, quantity);
    else if (id > root->foodId)
        root->right = insertMenuItem(root->right, id, name, price, quantity);
    return root;
}

MenuItem* searchMenuItem(MenuItem* root, int id) {
    if (root == nullptr || root->foodId == id)
        return root;
    if (id < root->foodId)
        return searchMenuItem(root->left, id);
    else
        return searchMenuItem(root->right, id);
}

int generateOrderId() {
    static int id = 1000;
    return id++;
}

void placeOrder(MenuItem* root, string customerName, int foodId, int quantity, string orderDate) {
    MenuItem* item = searchMenuItem(root, foodId);
    if (item == nullptr) {
        cout << "Item not found in menu.\n";
        return;
    }
    if (item->quantity < quantity) {
        cout << "Not enough stock for item: " << item->name << endl;
        return;
    }
    item->quantity -= quantity;
    int id = generateOrderId();
    Order newOrder(id, customerName, foodId, quantity, orderDate);
    orders.push(newOrder);
    cout << "Order placed successfully Order ID: " << id << endl;
}

int main() {
    MenuItem* menu = nullptr;
    menu = insertMenuItem(menu, 101, "Burger", 300, 10);
    menu = insertMenuItem(menu, 102, "Pizza", 500, 5);
    menu = insertMenuItem(menu, 103, "Fries", 150, 15);

    placeOrder(menu, "Ali", 101, 2, "18/05/2025");
    placeOrder(menu, "Hassan", 102, 1, "14/03/2025");
    placeOrder(menu, "Farhan", 105, 1, "10/01/2025");
    placeOrder(menu, "Ahmad", 101, 20, "18052025");

    return 0;
}
