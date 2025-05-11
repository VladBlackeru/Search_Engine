//
// Created by Vlod on 05/12/2025.
//

#ifndef APP_QUERYPARSER_H
#define APP_QUERYPARSER_H

#include <string>
#include <vector>

struct QueryTerms {
    std::vector<std::string> pathTerms;
    std::vector<std::string> contentTerms;
};

class QueryParser {
public:
    static QueryTerms parse(const std::string &query);
};


#endif //APP_QUERYPARSER_H
