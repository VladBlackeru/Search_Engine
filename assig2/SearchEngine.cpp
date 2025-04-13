#include "SearchEngine.h"
#include <algorithm>
#include <iostream>
#include <fstream>

std::unordered_map<std::string, std::vector<SearchResult>> resultCache;
bool containsQuery(const std::string& text, const std::string& query) {
    std::string lowText = text;
    std::string lowQuery = query;
    std::transform(lowText.begin(), lowText.end(), lowText.begin(), ::tolower);
    std::transform(lowQuery.begin(), lowQuery.end(), lowQuery.begin(), ::tolower);
    return (lowText.find(lowQuery) != std::string::npos);
}

void searchWorker(const fs::path& rootDir, const std::string& query, std::vector<SearchResult>& localResults) {
    try {
        if (fs::exists(rootDir) && fs::is_directory(rootDir)) {
            for (const auto& entry : fs::recursive_directory_iterator(rootDir)) {
                if (entry.is_regular_file()) {
                    std::ifstream inFile(entry.path());
                    if (!inFile) {
                        std::cerr << "Couldn't open file: " << entry.path() << std::endl;
                        continue;
                    }

                    std::string line;
                    size_t lineNum = 0;
                    while (std::getline(inFile, line)) {
                        lineNum++;
                        if (containsQuery(line, query)) {
                            localResults.push_back({ entry.path().string(), lineNum, line });
                        }
                    }
                }
            }
        } else {
            std::cerr << "Directory invalid: " << rootDir << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error working on directory " << rootDir << " : " << e.what() << std::endl;
    }
}



