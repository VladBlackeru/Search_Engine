#include "SearchHistory.h"

void SearchHistory::update(const std::string &query) {
    history.push_back(query);
}

void SearchHistory::printHistory() const {
    std::cout << "Search History:\n";
    for (const auto &q : history) {
        std::cout << q << "\n";
    }
}

void SearchHistory::suggestQueries() const {
    std::map<std::string, int> freq;
    for (const auto &q : history) {
        freq[q]++;
    }
    std::vector<std::pair<std::string,int>> suggestions(freq.begin(), freq.end());
    std::sort(suggestions.begin(), suggestions.end(), [](const auto &a, const auto &b){
        return a.second > b.second;
    });
    std::cout << "Query Suggestions:\n";
    int count = 0;
    for (const auto &item : suggestions) {
        std::cout << item.first << " (" << item.second << " times)\n";
        if (++count >= 3) break;
    }
}