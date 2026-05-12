#include <iostream>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

struct Medicine {
    int id;
    string name;
    string category;
    int quantity;
    double price;
};

void rebuildIndex(Medicine medicines[], int count, unordered_map<int, int>& idIndex) {
    idIndex.clear();

    for (int i = 0; i < count; i++) {
        idIndex[medicines[i].id] = i;
    }
}

void saveToFile(Medicine medicines[], int count) {
    ofstream file("medicines.txt");

    for (int i = 0; i < count; i++) {
        file << medicines[i].id << ","
            << medicines[i].name << ","
            << medicines[i].category << ","
            << medicines[i].quantity << ","
            << medicines[i].price << endl;
    }

    file.close();
}

void loadFromFile(Medicine medicines[], int& count, int capacity, unordered_map<int, int>& idIndex) {
    ifstream file("medicines.txt");

    if (!file) {
        cout << "No existing file found A new file will be created later \n";
        return;
    }

    count = 0;
    string line;

    while (getline(file, line) && count < capacity) {
        stringstream ss(line);

        string idStr;
        string name;
        string category;
        string quantityStr;
        string priceStr;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, category, ',');
        getline(ss, quantityStr, ',');
        getline(ss, priceStr, ',');

        if (!idStr.empty() && !quantityStr.empty() && !priceStr.empty()) {
            medicines[count].id = stoi(idStr);
            medicines[count].name = name;
            medicines[count].category = category;
            medicines[count].quantity = stoi(quantityStr);
            medicines[count].price = stod(priceStr);

            count++;
        }
    }

    file.close();
    rebuildIndex(medicines, count, idIndex);

    cout << "Data loaded successfully.\n";
}

void addMedicine(Medicine medicines[], int& count, int capacity, unordered_map<int, int>& idIndex) {
    if (count >= capacity) {
        cout << "Inventory is full Cannot - add more medicines \n";
        return;
    }

    Medicine m;

    cout << "Enter medicine ID: ";
    cin >> m.id;

    if (idIndex.find(m.id) != idIndex.end()) {
        cout << "Medicine ID already exists \n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter medicine name: ";
    getline(cin, m.name);

    cout << "Enter category: ";
    getline(cin, m.category);

    cout << "Enter quantity: ";
    cin >> m.quantity;

    cout << "Enter price: ";
    cin >> m.price;

    medicines[count] = m;
    count++;

    rebuildIndex(medicines, count, idIndex);
    saveToFile(medicines, count);

    cout << "Medicine added successfully \n";
}

void editMedicine(Medicine medicines[], int count, unordered_map<int, int>& idIndex) {
    int id;

    cout << "Enter medicine ID to edit: ";
    cin >> id;

    if (idIndex.find(id) == idIndex.end()) {
        cout << "Medicine not found \n";
        return;
    }

    int index = idIndex[id];

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter new medicine name: ";
    getline(cin, medicines[index].name);

    cout << "Enter new category: ";
    getline(cin, medicines[index].category);

    cout << "Enter new quantity: ";
    cin >> medicines[index].quantity;

    cout << "Enter new price: ";
    cin >> medicines[index].price;

    saveToFile(medicines, count);

    cout << "Medicine updated successfully.\n";
}

void deleteMedicine(Medicine medicines[], int& count, unordered_map<int, int>& idIndex) {
    int id;

    cout << "Enter medicine ID to delete: ";
    cin >> id;

    if (idIndex.find(id) == idIndex.end()) {
        cout << "Medicine not found.\n";
        return;
    }

    int index = idIndex[id];

    for (int i = index; i < count - 1; i++) {
        medicines[i] = medicines[i + 1];
    }

    count--;

    rebuildIndex(medicines, count, idIndex);
    saveToFile(medicines, count);

    cout << "Medicine deleted successfully \n";
}

void searchMedicine(Medicine medicines[], int count, unordered_map<int, int>& idIndex) {
    int id;

    cout << "Enter medicine ID to search: ";
    cin >> id;

    if (idIndex.find(id) == idIndex.end()) {
        cout << "Medicine not found.\n";
        return;
    }

    int index = idIndex[id];

    cout << "\nMedicine Found:\n";
    cout << "ID: " << medicines[index].id << endl;
    cout << "Name: " << medicines[index].name << endl;
    cout << "Category: " << medicines[index].category << endl;
    cout << "Quantity: " << medicines[index].quantity << endl;
    cout << "Price: " << medicines[index].price << endl;
}

void printMedicines(Medicine medicines[], int count) {
    if (count == 0) {
        cout << "No medicines available.\n";
        return;
    }

    cout << "\nAll Medicines:\n";
    cout << "----------------------------------\n";

    for (int i = 0; i < count; i++) {
        cout << "ID: " << medicines[i].id << endl;
        cout << "Name: " << medicines[i].name << endl;
        cout << "Category: " << medicines[i].category << endl;
        cout << "Quantity: " << medicines[i].quantity << endl;
        cout << "Price: " << medicines[i].price << endl;
        cout << "----------------------------------\n";
    }
}

void printLowStockMedicines(Medicine medicines[], int count) {
    queue<int> lowStockQueue;

    for (int i = 0; i < count; i++) {
        if (medicines[i].quantity < 10) {
            lowStockQueue.push(i);
        }
    }

    if (lowStockQueue.empty()) {
        cout << "No low-stock medicines.\n";
        return;
    }

    cout << "\nLow Stock Medicines:\n";
    cout << "----------------------------------\n";

    while (!lowStockQueue.empty()) {
        int index = lowStockQueue.front();
        lowStockQueue.pop();

        cout << "ID: " << medicines[index].id << endl;
        cout << "Name: " << medicines[index].name << endl;
        cout << "Quantity: " << medicines[index].quantity << endl;
        cout << "----------------------------------\n";
    }
}

void showMenu() {
    cout << "\n-- Pharmacy Inventory Management System --\n";
    cout << "1- Add Medicine\n";
    cout << "2- Edit Medicine\n";
    cout << "3- Delete Medicine\n";
    cout << "4- Search Medicine\n";
    cout << "5- Print All Medicines\n";
    cout << "6- Print Low Stock Medicines\n";
    cout << "7- Exit\n";
    cout << "Choose an option: ";
}

int main() {
    const int capacity = 100;

    Medicine medicines[capacity];
    int count = 0;

    unordered_map<int, int> idIndex;

    loadFromFile(medicines, count, capacity, idIndex);

    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            addMedicine(medicines, count, capacity, idIndex);
            break;

        case 2:
            editMedicine(medicines, count, idIndex);
            break;

        case 3:
            deleteMedicine(medicines, count, idIndex);
            break;

        case 4:
            searchMedicine(medicines, count, idIndex);
            break;

        case 5:
            printMedicines(medicines, count);
            break;

        case 6:
            printLowStockMedicines(medicines, count);
            break;

        case 7:
            saveToFile(medicines, count);
            cout << "Data saved Exiting program.\n";
            break;

        default:
            cout << "Invalid choice Please try again.\n";
        }

    } while (choice != 7);

    return 0;
}