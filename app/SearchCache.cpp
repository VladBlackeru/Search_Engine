#include "SearchCache.h"

SearchCache::SearchCache() {
    size_of_cache = 0;
}

SearchCache& SearchCache::getInstance() {
    static SearchCache instance;
    return instance;
}

bool SearchCache::hasCachedResult(const std::string& query) {
    return cache.find(query) != cache.end();
}

void SearchCache::storeResult(const std::string& query, const std::vector<SearchResult>& results) {
    cache[query] = results;
}

std::vector<SearchResult> SearchCache::getCachedResult(const std::string& query) {
    return cache[query];
}