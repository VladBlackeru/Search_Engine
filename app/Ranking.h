#ifndef APP_RANKING_H
#define APP_RANKING_H

#include "QueryParser.h"

struct QueryTerms;

struct SearchResult {
    std::string content;
    int line_number;
    std::string file_name;
    std::string file_path;
    long file_size;
    std::string file_extension;
    double score = 0.0;
};

class Ranking {
public:
    static double computeScore(const SearchResult &result, const QueryTerms &query);
};

#endif //APP_RANKING_H
