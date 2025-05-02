#include <iostream>
#include <vector>
#include <string>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"

using namespace std;

// ---------------------- Customer Class ----------------------
class Customer {
private:
    int id;
    string name;
    double balance;

public:
    Customer(int id, string name, double balance) : id(id), name(name), balance(balance) {}

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    double getBalance() const {
        return balance;
    }

    void setName(const string& newName) {
        name = newName;
    }

    void setBalance(double newBalance) {
        balance = newBalance;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount > balance) return false;
        balance -= amount;
        return true;
    }
};

// ---------------------- Admin Class ----------------------
class Admin {
private:
    vector<Customer*> customers;

public:
    void addCustomer(int id, string name, double balance) {
        for(auto c : customers) {
            if(c->getId() == id) {
                cout << RED << "Customer with ID " << id << " already exists.\n" << RESET;
                cin.ignore();
                cout << YELLOW << "Press Enter to continue...\n" << RESET;
                cin.get();
                return;
            }
        }
        Customer* newCustomer = new Customer(id, name, balance);
        customers.push_back(newCustomer);
        cout << GREEN << "Customer added successfully.\n" << RESET;
        cin.ignore();
        cout << YELLOW << "Press Enter to continue...\n" << RESET;
        cin.get();
    }


    void viewCustomers() const {
        if(customers.empty()) {
            cout << RED << "No customers found.\n" << RESET;
            cin.ignore();
            cout << YELLOW << "Press Enter to continue...\n" << RESET;
            cin.get();
            return;
        }
        for(auto c : customers) {
            cout << CYAN
                << "ID: " << c->getId()
                << ", Name: " << c->getName()
                << ", Balance: $" << c->getBalance() << "\n"
                << RESET;
        }
        cin.ignore();
        cout << YELLOW << "Press Enter to continue...\n" << RESET;
        cin.get();
    }

    bool updateCustomer(int id, string newName) {
        for(auto c : customers) {
            if(c->getId() == id) {
                c->setName(newName);
                return true;
            }
        }
        return false;
    }

    bool deleteCustomer(int id) {
        for(auto it = customers.begin(); it != customers.end(); ++it) {
            if((*it)->getId() == id) {
                delete *it;
                customers.erase(it);
                cout << GREEN << "Customer deleted successfully.\n" << RESET;
                return true;
            }
        }
        return false;
    }
    

    bool transferMoney(int fromId, int toId, double amount) {
        Customer *from = nullptr;
        Customer *to = nullptr;
        for(auto c : customers) {
            if(c->getId() == fromId) from = c;
            if(c->getId() == toId) to = c;
        }
    
        if(from && to && from->withdraw(amount)) {
            to->deposit(amount);
            cout << GREEN << "Transfer successful.\n" << RESET;
            return true;
        }
    
        return false;
    }

    ~Admin() {
        for (auto c : customers) {
            delete c;
        }
        cout << GREEN << "Memory Cleared\n\n" << RESET;
    }
};

// ---------------------- Main Function ----------------------
int main() {
    Admin admin;
    int choice;

    do {
        cout << "\nPayment System Menu:\n";
        cout << "1. Add Customer\n2. View Customers\n3. Update Customer\n";
        cout << "4. Delete Customer\n5. Transfer Money\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        int id, id2;
        string name;
        double balance, amount;

        switch(choice) {
        case 1:
            cout << "Enter ID, Name, Balance: ";
            cin >> id >> name >> balance;
            admin.addCustomer(id, name, balance);
            break;

        case 2:
            admin.viewCustomers();
            break;

        case 3:
            cout << "Enter ID and new Name: ";
            cin >> id >> name;
            if(!admin.updateCustomer(id, name))
                cout << RED << "Customer not found.\n" << RESET;
            else cout << GREEN << "Updated Successfully." << RESET;
            break;

        case 4:
            cout << "Enter ID to delete: ";
            cin >> id;
            if(!admin.deleteCustomer(id))
                cout << RED << "Customer not found.\n" << RESET;
            break;

        case 5:
            cout << "Enter From ID, To ID, Amount: ";
            cin >> id >> id2 >> amount;
            if (!admin.transferMoney(id, id2, amount))
                cout << RED << "Transfer failed (invalid ID or insufficient balance).\n" << RESET;
            break;
        }
    } while(choice != 0);

    return 0;
}
