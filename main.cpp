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
void printProducts(const vector<product> &products);

int main() {

    vector<product> products;

    if (readProducts(products)) {
        printProducts(products);
    }
    else {
        cout << "Error reading input." << endl;
    }

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
void printProducts(const vector<product> &products) {
    ofstream output;
    output.open("output.txt");
    output << fixed << setprecision(2);
    for (size_t i = 0; i < products.size(); i++) {
        output << products[i].name << " "
        << products[i].quantity << " "
        << products[i].price << endl;
    }
    output.close();
}