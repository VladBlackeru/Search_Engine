#include "QueryParser.h"
#include <sstream>
#include <algorithm>

QueryTerms QueryParser::parse(const std::string &query) {
    QueryTerms terms;
    std::istringstream iss(query);
    std::string token;
    while (iss >> token) {
        size_t pos = token.find(':');
        if (pos != std::string::npos) {
            std::string qualifier = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            std::transform(qualifier.begin(), qualifier.end(), qualifier.begin(), ::tolower);
            if (qualifier == "path") {
                terms.pathTerms.push_back(value);
            } else if (qualifier == "content") {
                terms.contentTerms.push_back(value);
            }
        } else {
            terms.contentTerms.push_back(token);
        }
    }
    return terms;
}