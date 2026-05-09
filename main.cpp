#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cctype>
using namespace std;

class Product {
    private:
    string name;
    int quantity;
    double price;

    public:
    Product (string n, int q, double pr) {
        name = n;

        if (q < 0) {
            quantity = 0;
        }
        else {
            quantity = q;
        }
        if (pr < 0.0) {
            price = 0.0;
        }
        else {
            price = pr;
        }
    }
    //------
        void print(ofstream &output) const {
            output << left << setw(22) << name
            << setw(13) << quantity
            << setw(15) << price << endl;
        }
    double value() const {
        return price * quantity;
    }
    double getPrice() const {
        return price;
    }
    int getQuantity() const {
        return quantity;
    }
    bool isLowStock() const {
        return quantity < 5;
    }
    string getName() const {
        return name;
    }
};

bool readProducts(vector<Product> &products);
void printProducts(const vector<Product> &products,ofstream &output);
double totalInventoryValue(const vector<Product> &products);
void printSummary(const vector<Product> &products, const vector<Product> &lowStock, ofstream &output);
int mostExpensiveProduct(const vector<Product> &products);
vector <Product> findLowStock(const vector<Product> &products);
int findProductByName(const vector<Product> &products);
void sortByPrice(vector<Product> &products,bool ascending);
void sortByQuantity(vector<Product> &products,bool ascending);
void handleSorting(vector<Product> &products);
void handleSearch(const vector<Product> &products);
void printFullReport(const vector<Product> &products);

int main() {
    vector<Product> products;
    bool running = true;
    if (readProducts(products)) {
        while (running) {
            cout << "< Menu >" << endl;
            cout << "-------------------"<< endl;
            cout << "1. Print full product report" << endl;
            cout << "2. Sort Products" << endl;
            cout << "3. Search Products" << endl;
            cout << "4. Quit" << endl;
            cout << "\nSelect an option 1-4: ";
            int choice;

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input.Enter an integer number." << endl;
                continue;
            }
            switch (choice) {
                case 1:
                    printFullReport(products);
                    cout << "Product report written to output.txt" << endl;
                    break;
                case 2:
                    handleSorting(products);
                    break;
                case 3:
                    handleSearch(products);
                    break;
                case 4:
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
    return 0;
}

bool readProducts(vector<Product> &products) {
    ifstream input;
    input.open("input.txt");

    if (input.fail()) {
        return false;
    }
        string name;
        int quantity;
        double price;

        while (input >> name >> quantity >> price) {
            Product temp(name,quantity,price);
            products.push_back(temp);
        }
    input.close();
    return true;
}
void printProducts(const vector<Product> &products,ofstream &output) {
    output << fixed << setprecision(2);
    output << left << setw(15) << "Product name" << setw(4) << "|"
    << setw(11) << "Quantity"
    << setw(5) << "|" << "Price" << endl;
    output << "<------------------------------------------------->" << endl;
    for (size_t i = 0; i < products.size(); i++) {
        products[i].print(output);
    }
}
double totalInventoryValue(const vector<Product> &products) {
    double totalSum = 0.0;
    for (const auto& product: products) {
        totalSum += product.value();
    }
    return totalSum;
}
void printSummary(const vector<Product> &products, const vector<Product> &lowStock, ofstream &output) {
    int ExpensiveProduct = mostExpensiveProduct(products);
    if (ExpensiveProduct == -1) {
        output << "No products available." << endl;
    }
    else {
        output << endl;
        output << "<-------------< Product Summary >----------------->" << endl;
        output << endl;
        output << "Total price of the product inventory: " << totalInventoryValue(products) << endl;
        output << "Most Expensive Product: " << products[ExpensiveProduct].getName()
        << ", its price is "<< products[ExpensiveProduct].getPrice() << endl;
        output << endl;

        output << "<-------< Products that are low on stock >-------->" << endl;
        output << left << setw(15) << "Product name" << setw(4) << "|"
        << setw(11) << "Quantity" << setw(6) << "|" << endl;
        for (const auto& l: lowStock) {
            output << left << setw(22) << l.getName()
            << setw(15) << l.getQuantity() << endl;
        }
    }
}
int mostExpensiveProduct(const vector<Product> &products) {
    if (products.empty()) {
        return -1;
    }
    double mostPrice = products[0].getPrice();
    int index = 0;
    for (size_t i = 1; i < products.size(); i++) {
        if (products[i].getPrice() > mostPrice) {
            mostPrice = products[i].getPrice();
            index = i;
        }
    }
    return index;
}
vector <Product> findLowStock(const vector<Product> &products) {
    vector<Product> lowStock;
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].isLowStock()) {
            lowStock.push_back(products[i]);
        }
    }
    return lowStock;
}
int findProductByName(const vector<Product> &products) {
    string searchName;
    cout << "Enter Product Name: " << endl;
    cin >> searchName;
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].getName() == searchName) {
            return i;
        }
    }
    return -1;
}
void sortByPrice(vector<Product> &products, bool ascending) {
    for (size_t i = 0; i < products.size(); i++) {
        for (size_t j = i + 1; j < products.size(); j++) {
            if ((ascending && products[j].getPrice() < products[i].getPrice()) ||
                (!ascending && products[j].getPrice() > products[i].getPrice())) {

                Product temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
}
void sortByQuantity(vector<Product> &products,bool ascending) {
    for (size_t i = 0; i < products.size(); i++) {
        for (size_t j = i + 1; j < products.size(); j++) {
            if ((ascending && products[j].getQuantity() < products[i].getQuantity())||
                (!ascending && products[j].getQuantity() > products[i].getQuantity())) {
                Product temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
}
void handleSorting(vector<Product> &products) {
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
void handleSearch(const vector<Product> &products) {
    int productIndex = findProductByName(products);
    if (productIndex == -1) {
        cout << "Product not found." << endl;
    }
    else {
        cout << "\nSearch results:" << endl;
        cout << "Product was found." << endl;
        cout << "Name: " << products[productIndex].getName()
        << "\nQuantity: " << products[productIndex].getQuantity()
        << "\nPrice: " << products[productIndex].getPrice() << endl;
    }
}
void printFullReport(const vector<Product> &products) {
    ofstream output("output.txt");

    printProducts(products,output);

    vector<Product> lowStock = findLowStock(products);
    printSummary(products, lowStock, output);

    output.close();
}