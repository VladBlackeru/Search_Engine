#include "SearchCache.h"
#include "Utils.h"
SearchCache::SearchCache() {
    cache_size = 0;
}

SearchCache& SearchCache::getInstance() {
    static SearchCache instance;
    return instance;
}

bool SearchCache::hasCachedResult(const std::string& query) {
    return cache.find(query) != cache.end();
}

void SearchCache::storeResult(const std::string& query, const std::vector<SearchResult>& results) {
    if(cache_size > MAX_CACHE_SIZE)
        cache.clear();
    int size = 0;
    for(auto res: results)
        size += sizeof(res);
    if(size > MAX_CACHE_SIZE)
        return;
    if(size + cache_size > MAX_CACHE_SIZE){
        cache.clear();
        cache_size = 0;
    }
    cache_size += size;
    cache[query] = results;
}

std::vector<SearchResult> SearchCache::getCachedResult(const std::string& query) {
    return cache[query];
}