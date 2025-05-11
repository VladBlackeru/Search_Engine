#include "Database.h"
#include "FileRepository.h"
#include "Utils.h"
#include "SearchHistory.h"
#include <iostream>
#include <string>
#include <limits>

int main() {
    const std::string connInfo = "dbname=postgres user=postgres password=177013 hostaddr=127.0.0.1 port=5432";
    Database db(connInfo);
    FileRepository repository(db);
    SearchHistory searchHistory;
    repository.registerSearchObserver(&searchHistory);
    int op = -1;
    std::string path;

    std::cout << "Menu:\n";
    std::cout << " 1 - Insert 1 file into DB\n";
    std::cout << " 2 - Insert Recursively into DB\n";
    std::cout << " 3 - Delete files\n";
    std::cout << " 4 - Search in files\n";
    std::cout << " 5 - Advanced search\n";
    std::cout << " 6 - View search history\n";
    std::cout << " 0 - Exit\n\n";

    do {
        std::cout << "Enter your choice: ";
        std::cin >> op;

        // Check for input failure
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number corresponding to the menu options.\n";
            continue;
        }

        // Clear any remaining characters in the input buffer
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
            case 5: {
                std::string queryStr;
                std::cout << "Enter advanced search query (e.g., 'path:docs content:report'): ";
                std::getline(std::cin, queryStr);
                repository.searchFilesAdvanced(queryStr);
                break;
            }
            case 6:
                searchHistory.printHistory();
                searchHistory.suggestQueries();
                break;
            case 0:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid option." << std::endl;
        }
    } while (op != 0);

    return 0;
}
