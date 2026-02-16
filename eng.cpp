#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// 1. Create Class based on the structure
class User {
public:
    string bankID;
    string username;    // [First - Last Name]
    string password;    // Password / Pin
    int failedAttempts; // Number of failed login attempts
    bool isLocked;      // Account lock status (if failed over limit)

    // Constructor
    User(string id, string name, string pass, int fails = 0) {
        bankID = id;
        username = name;
        password = pass;
        failedAttempts = fails;
        isLocked = (failedAttempts >= 3); // Lock if failed >= 3 times
    }
};

// Global Vector to store all users
vector<User> users;
const string FILENAME = "users.csv";

// --- CSV File Management ---

void saveUsersToFile() {
    ofstream file(FILENAME);
    if (file.is_open()) {
        for (const auto& u : users) {
            // Format: BankID,Name,Pass,FailCount
            file << u.bankID << "," 
                 << u.username << "," 
                 << u.password << "," 
                 << u.failedAttempts << "\n";
        }
        file.close();
    } else {
        cout << "Error: Cannot save file!" << endl;
    }
}

void loadUsersFromFile() {
    users.clear(); 
    ifstream file(FILENAME);
    string line;

    if (!file.is_open()) {
        cout << "Warning: Data file not found. Starting new system..." << endl;
        return;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, pass, failStr;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, pass, ',');
        getline(ss, failStr, ',');

        if (!id.empty()) {
            int fails = stoi(failStr);
            users.push_back(User(id, name, pass, fails));
        }
    }
    file.close();
}

// --- System Functions ---

int findUserIndex(string id) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].bankID == id) {
            return i;
        }
    }
    return -1;
}

void loginSystem() {
    string inputID, inputPass;
    
    cout << "\n=== LOGIN SYSTEM ===" << endl;
    cout << "Enter Bank-ID: ";
    cin >> inputID;

    int index = findUserIndex(inputID);

    if (index == -1) {
        cout << "X User not found in the system." << endl;
        return;
    }

    if (users[index].isLocked) {
        cout << "!!! This account has been temporarily locked (Failed login over 3 times)." << endl;
        cout << "Please contact staff." << endl;
        return;
    }

    cout << "Enter Password/Pin: ";
    cin >> inputPass;

    if (inputPass == users[index].password) {
        cout << "\n---------------------------------" << endl;
        cout << "Login Success! Welcome " << users[index].username << endl;
        cout << "---------------------------------" << endl;
        
        users[index].failedAttempts = 0;
        users[index].isLocked = false;
        saveUsersToFile();
        
        // TODO: Add main banking menu here (Deposit/Withdraw etc.)
    } else {
        users[index].failedAttempts++;
        cout << "\nX Incorrect password! (" 
             << users[index].failedAttempts << "/3)" << endl;

        if (users[index].failedAttempts >= 3) {
            users[index].isLocked = true;
            cout << "!!! Your account has been locked !!!" << endl;
        }
        
        saveUsersToFile();
    }
}

// Dummy data (for first run)
void createDummyData() {
    users.push_back(User("101", "Somchai_Jaidee", "1234", 0));
    users.push_back(User("102", "Somsri_Rakrian", "5678", 0));
    saveUsersToFile();
    cout << "Dummy data created (User 101, 102)" << endl;
}

int main() {
    loadUsersFromFile();

    // Uncomment this once for first-time setup
    // createDummyData(); 

    while (true) {
        cout << "\n==== MAIN MENU ====" << endl;
        cout << "1. Login" << endl;
        cout << "2. Exit Program" << endl;
        cout << "Select: ";
        
        int choice;
        cin >> choice;

        if (choice == 1) {
            loginSystem();
        } else if (choice == 2) {
            cout << "Closing program..." << endl;
            break;
        } else {
            cout << "Invalid selection. Please try again." << endl;
        }
    }

    return 0;
}
