#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <limits>  // For numeric_limits<streamsize>::max()

using namespace std;

class DirectoryEntry {
public:
    char name[20];
    char address[20];
    unsigned int phoneNumber;

    void get();
    void show();
    void modify();
};

void DirectoryEntry::get() {
    cout << "\nEnter name: ";
    cin.getline(name, 20);

    cout << "Enter address: ";
    cin.getline(address, 20);

    cout << "Enter phone number: ";
    cin >> phoneNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Flush the input buffer
}

void DirectoryEntry::show() {
    cout << "\nName: " << name;
    cout << "\nAddress: " << address;
    cout << "\nPhone Number: " << phoneNumber << "\n";
}

int main() {
    DirectoryEntry entry;
    fstream file;
    int choice;

    while (true) {
        cout << "\n--- Telephone Directory ---\n";
        cout << "1. Add Entry\n";
        cout << "2. Show All Entries\n";
        cout << "3. Search Entry\n";
        cout << "4. Modify Entry\n";
        cout << "5. Delete Entry\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Flush the input buffer

        switch (choice) {
            case 1: {
                file.open("directory.dat", ios::out | ios::app | ios::binary);
                if (!file) {
                    cerr << "Error opening file!\n";
                    exit(1);
                }
                entry.get();
                file.write(reinterpret_cast<char*>(&entry), sizeof(entry));
                file.close();
                break;
            }
            case 2: {
                file.open("directory.dat", ios::in | ios::binary);
                if (!file) {
                    cerr << "Error opening file!\n";
                    exit(1);
                }
                while (file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
                    entry.show();
                }
                file.close();
                break;
            }
            case 3: {
                char searchName[20];
                cout << "Enter the name to search: ";
                cin.getline(searchName, 20);

                file.open("directory.dat", ios::in | ios::binary);
                if (!file) {
                    cerr << "Error opening file!\n";
                    exit(1);
                }

                bool found = false;
                while (file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
                    if (strcmp(entry.name, searchName) == 0) {
                        entry.show();
                        found = true;
                        break;
                    }
                }
                file.close();

                if (!found) {
                    cout << "No entry found for " << searchName << "\n";
                }
                break;
            }
            case 4: {
                unsigned int searchPhone;
                cout << "Enter the phone number to modify: ";
                cin >> searchPhone;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Flush the input buffer

                fstream tempFile("temp.dat", ios::out | ios::binary);
                file.open("directory.dat", ios::in | ios::binary);

                if (!file || !tempFile) {
                    cerr << "Error opening file!\n";
                    exit(1);
                }

                bool modified = false;
                while (file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
                    if (entry.phoneNumber == searchPhone) {
                        cout << "Current details:\n";
                        entry.show();
                        cout << "\nEnter new details:\n";
                        entry.get();
                        modified = true;
                    }
                    tempFile.write(reinterpret_cast<char*>(&entry), sizeof(entry));
                }
                file.close();
                tempFile.close();

                if (modified) {
                    if (remove("directory.dat") != 0) {
                        cerr << "Error deleting old file!\n";
                    } else if (rename("temp.dat", "directory.dat") != 0) {
                        cerr << "Error renaming file!\n";
                    } else {
                        cout << "Record modified successfully.\n";
                    }
                } else {
                    cout << "No record found with the given phone number.\n";
                }
                break;
            }
            case 5: {
                char deleteName[20];
                cout << "Enter the name to delete: ";
                cin.getline(deleteName, 20);

                fstream tempFile("temp.dat", ios::out | ios::binary);
                file.open("directory.dat", ios::in | ios::binary);

                if (!file || !tempFile) {
                    cerr << "Error opening file!\n";
                    exit(1);
                }

                bool deleted = false;
                while (file.read(reinterpret_cast<char*>(&entry), sizeof(entry))) {
                    if (strcmp(entry.name, deleteName) != 0) {
                        tempFile.write(reinterpret_cast<char*>(&entry), sizeof(entry));
                    } else {
                        deleted = true;
                    }
                }
                file.close();
                tempFile.close();

                if (deleted) {
                    if (remove("directory.dat") != 0) {
                        cerr << "Error deleting old file!\n";
                    } else if (rename("temp.dat", "directory.dat") != 0) {
                        cerr << "Error renaming file!\n";
                    } else {
                        cout << "Record deleted successfully.\n";
                    }
                } else {
                    cout << "No record found with the given name.\n";
                }
                break;
            }
            case 6:
                exit(0);
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
