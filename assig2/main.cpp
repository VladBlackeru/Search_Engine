#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "SearchEngine.h"
#include <filesystem>
#define MAX_CACHE_SIZE 2048

namespace fs = std::filesystem;

int main() {
    std::string rootPath;
    std::cout << "Enter directory for search: ";
    std::getline(std::cin, rootPath);

    while (!fs::exists(rootPath) || !fs::is_directory(rootPath)) {
        std::cerr << "Invalid directory, enter a new one: ";
        std::getline(std::cin, rootPath);
    }

    while (true) {
        std::cout << "\n--- Menu ---\n";
        std::cout << "1. search in current dir (" << rootPath << ")\n";
        std::cout << "2. Change dir\n";
        std::cout << "3. quit\n";
        std::cout << "choos 1-3: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "search query: ";
            std::string query;
            std::getline(std::cin, query);

            if (resultCache.find(query) != resultCache.end()) {
                std::cout << "cache result found." << std::endl;
                processAndDisplayResults(query, resultCache[query]);
                continue;
            }

            if(resultCache.size() > MAX_CACHE_SIZE)
                resultCache.clear();

            std::vector<fs::path> partitions = getPartitions(rootPath);

            std::vector<std::thread> threads;
            std::vector<std::vector<SearchResult>> localResults(partitions.size());

            for (size_t i = 0; i < partitions.size(); ++i) {
                threads.emplace_back(searchWorker, partitions[i], query, std::ref(localResults[i]));
            }

            std::vector<SearchResult> rootResults;
            std::thread rootThread(searchWorker, fs::path(rootPath), query, std::ref(rootResults));

            for (auto& th : threads) {
                if (th.joinable())
                    th.join();
            }
            if (rootThread.joinable())
                rootThread.join();

            std::vector<SearchResult> combinedResults;
            for (const auto& vec : localResults) {
                combinedResults.insert(combinedResults.end(), vec.begin(), vec.end());
            }
            combinedResults.insert(combinedResults.end(), rootResults.begin(), rootResults.end());

            processAndDisplayResults(query, combinedResults);

        } else if (choice == "2") {
            std::cout << "Enter the new dir: ";
            std::getline(std::cin, rootPath);
            while (!fs::exists(rootPath) || !fs::is_directory(rootPath)) {
                std::cerr << "Invalid directory, enter a new one: ";
                std::getline(std::cin, rootPath);
            }
            resultCache.clear();
            std::cout << "Dir changed changed to: " << rootPath << "\n";
        } else if (choice == "3") {
            std::cout << "quit.\n";
            break;
        } else {
            std::cerr << "bad choice, choose again 1-3.\n";
        }
    }

    return 0;
}
