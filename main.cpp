#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

struct Product {
    int id;
    std::string name;
    int quantity;
    double price;
};

class Warehouse {
private:
    std::vector<Product> products;
    std::string fileName = "database.txt";

    int getNextId() const {
        int maxId=0;

        for (const Product& p : products) {
            if (p.id > maxId) {
                maxId = p.id;
            }
        }

        return maxId+1;
    }

    
public:
    void loadFromFile() {
        std::ifstream file(fileName);

        if (!file.is_open()) {
            return;
        }

        products.clear();
        std::string line;

        while (std::getline(file, line)) {
            std::stringstream stream(line);
            std::string id;
            std::string name;
            std::string quantity;
            std::string price;

            if (std::getline(stream, id, '|') &&
                std::getline(stream, name, '|') &&
                std::getline(stream, quantity, '|') &&
                std::getline(stream, price)) {

                Product p;
                p.id = std::stoi(id);
                p.name= name;
                p.quantity = std::stoi(quantity);
                p.price = std::stod(price);

                products.push_back(p);
            }
        }
    }

    void saveToFile() const {
        std::ofstream file(fileName);

        if (!file.is_open()) {
            std::cout << "Ошибка: не удалось открыть файл для сохранения.\n";
            return;
        }

        for (const Product& p : products) {
            file << p.id << '|'
                 << p.name << '|'
                 << p.quantity << '|'
                 << p.price << '\n';
        }

        std::cout << "Данные сохранены в файл " << fileName << ".\n";
    }

    void addProduct() {
        Product p;
        p.id = getNextId();

        std::cout << "Введите название товара: ";
        std::getline(std::cin, p.name);

        std::cout << "Введите количество: ";
        std::cin >> p.quantity;

        std::cout << "Введите цену: ";
        std::cin >> p.price;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        products.push_back(p);
        std::cout << "Товар добавлен. ID товара: " << p.id << "\n";
    }

    void showProducts() const {
        if (products.empty()) {
            std::cout << "Склад пуст.\n";
            return;
        }

        std::cout << std::left
                  << std::setw(8) << "ID"
                  << std::setw(25) << "Название"
                  << std::setw(15) << "Количество"
                  << std::setw(12) << "Цена" << '\n';

        std::cout << std::string(60, '-') << '\n';

        for (const Product& p : products) {
            std::cout << std::left
                      << std::setw(8) << p.id
                      << std::setw(25) << p.name
                      << std::setw(15) << p.quantity
                      << std::fixed << std::setprecision(2)
                      << std::setw(12) << p.price << '\n';
        }
    }
};

void showMenu() {
    std::cout << "\n=== Sklad ===\n";
    std::cout << "1 - Добавить товар\n";
    std::cout << "2 - Показать все товары\n";
    std::cout << "3 - Сохранить данные\n";
    std::cout << "0 - Выход\n";
    std::cout << "Выберите действие: ";
}

int main() {
    Warehouse warehouse;

    warehouse.loadFromFile();

    int choice = -1;

    while (true) {
        showMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice==1) {
            warehouse.addProduct();
        } else if (choice==2) {
            warehouse.showProducts();
        } else if (choice==3) {
            warehouse.saveToFile();
        } else if (choice==0) {
            std::cout << "Выход из программы.\n";
            break;
        } else {
            std::cout << "Неверный пункт меню. Попробуйте снова.\n";
        }
    }

    return 0;
}
