#include "Database.h"
#include "FileRepository.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <limits>

int main() {
    const std::string connInfo = "dbname=postgres user=postgres password=177013 hostaddr=127.0.0.1 port=5432";
    Database db(connInfo);
    FileRepository repository(db);

    int op;
    std::string path;

    std::cout << "Menu:\n";
    std::cout << " 1 - Insert 1 file into DB\n";
    std::cout << " 2 - Insert Recursively into DB\n";
    std::cout << " 3 - Delete files\n";
    std::cout << " 4 - Search in files\n";
    std::cout << " 0 - Exit\n\n";

    do {
        std::cin >> op;
        // Clear any newline characters for getline usage.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (op) {
            case 1:
                std::cout << "Enter file path: ";
                std::getline(std::cin, path);
                repository.insertFile(replaceBackslashes(path));
                break;
            case 2:
                std::cout << "Enter directory path: ";
                std::getline(std::cin, path);
                repository.insertRecursively(path);
                break;
            case 3:
                repository.deleteTableContents("search_engine.files");
                break;
            case 4: {
                std::string keyword;
                std::cout << "Enter search keyword: ";
                std::getline(std::cin, keyword);
                repository.searchFiles(keyword);
                break;
            }
            case 0:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid option." << std::endl;
        }
    } while (op != 0);

    return 0;
}