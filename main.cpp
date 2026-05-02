#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

struct product {
    string name;
    int quantity;
    double price;
};

bool readProducts(vector<product> &products);
void printProducts(const vector<product> &products, ofstream &output);
double totalInventoryValue(const vector<product> &products);
void printSummary(const vector<product> &products, const vector<product> &lowStock, ofstream &output);
int mostExpensiveProduct(const vector<product> &products);
vector <product> findLowStock(const vector<product> &products);
int findProductByName(const vector<product> &products);

int main() {
    ofstream output("output.txt");
    vector<product> products;
    if (readProducts(products)) {
        vector<product> lowStock = findLowStock(products);
        printProducts(products,output);
        printSummary(products,lowStock,output);
        int productIndex = findProductByName(products);
        if (productIndex == -1) {
            cout << "Product not found." << endl;
        }
        else {
            cout << "Product was found." << endl;
            cout << "Name: " << products[productIndex].name
            << "\nQuantity: " << products[productIndex].quantity
            << "\nPrice: " << products[productIndex].price;
        }
    }
    else {
        cout << "Error reading input." << endl;
    }
    output.close();
    return 0;
}
bool readProducts(vector<product> &products) {
    ifstream input;
    input.open("input.txt");

    if (input.fail()) {
        return false;
    }
        product temp;
        while (input >> temp.name >> temp.quantity >> temp.price) {
            products.push_back(temp);
        }
    input.close();
    return true;
}
void printProducts(const vector<product> &products, ofstream &output) {
    output << fixed << setprecision(2);
    output << left << setw(15) << "Product name" << setw(4) << "|"
    << setw(11) << "Quantity"
    << setw(5) << "|" << "Price" << endl;
    output << "<------------------------------------------------->" << endl;
    for (size_t i = 0; i < products.size(); i++) {
        output << left << setw(22) << products[i].name
        << setw(13) << products[i].quantity
        << setw(15) << products[i].price << endl;
    }
}
double totalInventoryValue(const vector<product> &products) {
    double totalSum = 0.0;
    for (const auto& p: products) {
        totalSum += p.quantity * p.price;
    }
    return totalSum;
}
void printSummary(const vector<product> &products, const vector<product> &lowStock, ofstream &output) {
    int ExpensiveProduct = mostExpensiveProduct(products);
    if (ExpensiveProduct == -1) {
        output << "No products available." << endl;
    }
    else {
        output << endl;
        output << "<-------------< Product Summary >----------------->" << endl;
        output << endl;
        output << "Total price of the product inventory: " << totalInventoryValue(products) << endl;
        output << "Most Expensive Product: " << products[ExpensiveProduct].name
        << ", its price is "<< products[ExpensiveProduct].price << endl;
        output << endl;

        output << "<-------< Products that are low on stock >-------->" << endl;
        output << left << setw(15) << "Product name" << setw(4) << "|"
        << setw(11) << "Quantity" << setw(6) << "|" << endl;
        for (const auto& l: lowStock) {
            output << left << setw(22) << l.name
            << setw(15) << l.quantity << endl;
        }
    }
}
int mostExpensiveProduct(const vector<product> &products) {
    if (products.empty()) {
        return -1;
    }
    double mostPrice = products[0].price;
    int index = 0;
    for (size_t i = 1; i < products.size(); i++) {
        if (products[i].price > mostPrice) {
            mostPrice = products[i].price;
            index = i;
        }
    }
    return index;
}
vector <product> findLowStock(const vector<product> &products) {
    vector <product> lowStock;
    int threshold = 5;
    for (int i = 0; i < products.size(); i++) {
        if (products[i].quantity < threshold) {
            lowStock.push_back(products[i]);
        }
    }
    return lowStock;
}
int findProductByName(const vector<product> &products) {
    string name;
    cout << "Enter Product Name: " << endl;
    cin >> name;
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].name == name) {
            return i;
        }
    }
    return -1;
}