#ifndef APP_SEARCHCACHE_H
#define APP_SEARCHCACHE_H
#include <string>
#include <map>
#include <vector>
#include "Ranking.h"

class SearchCache {
public:
    static SearchCache& getInstance();
    bool hasCachedResult(const std::string& query);
    void storeResult(const std::string& query, const std::vector<SearchResult>& results);
    std::vector<SearchResult> getCachedResult(const std::string& query);

private:
    SearchCache(); // Constructor is private
    SearchCache(const SearchCache&) = delete; // Delete copy constructor
    SearchCache& operator=(const SearchCache&) = delete; // Delete assignment operator

    int size_of_cache;
    std::map<std::string, std::vector<SearchResult>> cache;
};


#endif //APP_SEARCHCACHE_H
