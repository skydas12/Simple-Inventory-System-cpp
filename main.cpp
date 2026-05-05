#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cctype>
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
void sortByPrice(vector<product> &products,bool ascending);
void sortByQuantity(vector<product> &products,bool ascending);
void handleSorting(vector<product> &products,ofstream &output);
void handleSearch(const vector<product> &products);

int main() {
    ofstream output("output.txt");
    vector<product> products;
    bool running = true;
    if (readProducts(products)) {
        while (running) {
            cout << "< Menu >" << endl;
            cout << "-------------------"<< endl;
            cout << "1. Print Products" << endl;
            cout << "2. Sort Products" << endl;
            cout << "3. Search Products" << endl;
            cout << "4. Print Summary" << endl;
            cout << "5. Quit" << endl;
            cout << "\nSelect an option 1-5: ";
            int choice;

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input.Enter an integer number." << endl;
                continue;
            }
            switch (choice) {
                case 1:
                    printProducts(products, output);
                    cout << "Products written to output.txt" << endl;
                    break;
                case 2:
                    handleSorting(products, output);
                    break;
                case 3:
                    handleSearch(products);
                    break;
                case 4: {
                    vector<product> lowStock = findLowStock(products);
                    printSummary(products, lowStock, output);
                    break;
                }
                case 5:
                    running = false;
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    break;
            }
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
void sortByPrice(vector<product> &products, bool ascending) {
    for (size_t i = 0; i < products.size(); i++) {
        for (size_t j = i + 1; j < products.size(); j++) {
            if (ascending && products[j].price < products[i].price ||
                (!ascending && products[j].price > products[i].price)) {

                product temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
}
void sortByQuantity(vector<product> &products,bool ascending) {
    for (size_t i = 0; i < products.size(); i++) {
        for (size_t j = i + 1; j < products.size(); j++) {
            if (ascending && products[j].quantity < products[i].quantity ||
                (!ascending && products[j].quantity > products[i].quantity)) {
                product temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
}
void handleSorting(vector<product> &products, ofstream &output) {
        cout << "By what do you wish to sort:\n"
                "- Price (p)\n"
                "- Quantity (q)" << endl;
        char sortChoice;
        cin >> sortChoice;
        sortChoice = tolower(sortChoice);
        cout << "Ascending/Descending? (a/d)"<< endl;
        char orderChoice;
        cin >> orderChoice;
        orderChoice = tolower(orderChoice);

        //-----------------
        bool ascending = (orderChoice == 'a');
        if (orderChoice != 'a' && orderChoice != 'd') {
            cout << "Invalid order choice. Defaulting to ascending." << endl;
        }
        //------------------
        if (sortChoice == 'p') {
            sortByPrice(products,ascending);
            cout << "Sorted by price." << endl;
        }
        else if (sortChoice == 'q') {
            sortByQuantity(products,ascending);
            cout << "Sorted by quantity." << endl;
        }
        else {
            cout << "Invalid sort choice." << endl;
        }
}
void handleSearch(const vector<product> &products) {
    int productIndex = findProductByName(products);
    if (productIndex == -1) {
        cout << "Product not found." << endl;
    }
    else {
        cout << "\nSearch results:" << endl;
        cout << "Product was found." << endl;
        cout << "Name: " << products[productIndex].name
        << "\nQuantity: " << products[productIndex].quantity
        << "\nPrice: " << products[productIndex].price << endl;
    }
}