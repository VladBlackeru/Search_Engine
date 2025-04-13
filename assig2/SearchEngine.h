
#ifndef ASSIG2_SEARCHENGINE_H
#define ASSIG2_SEARCHENGINE_H

#include <string>
#include <vector>
#include <mutex>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

struct SearchResult {
    std::string filePath;
    size_t lineNumber;
    std::string lineText;
};

extern std::unordered_map<std::string, std::vector<SearchResult>> resultCache;

bool containsQuery(const std::string& text, const std::string& query);

void searchWorker(const fs::path& rootDir, const std::string& query, std::vector<SearchResult>& localResults);

void processAndDisplayResults(const std::string& query, std::vector<SearchResult>& allResults);

std::vector<fs::path> getPartitions(const fs::path& rootDir);


#endif //ASSIG2_SEARCHENGINE_H
