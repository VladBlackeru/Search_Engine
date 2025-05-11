#include "Ranking.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

static std::string toLower(const std::string &s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

double Ranking::computeScore(const SearchResult &result, const QueryTerms &query) {
    double score = 0.0;

    // 1. Inverse of path length (shorter paths is better):
    score += 100.0 / (result.file_path.length() + 1);

    // 2. Bonus if the file’s path contains one or more of the query’s path terms.
    std::string lowerPath = toLower(result.file_path);
    for (const auto &term : query.pathTerms) {
        std::string lowerTerm = toLower(term);
        if (lowerPath.find(lowerTerm) != std::string::npos) {
            score += 20.0;
        }
    }

    // 3. File extension prioritization.
    std::vector<std::string> prioritizedExtensions = {".txt", ".md", ".cpp", ".h"};
    for (const auto &ext : prioritizedExtensions) {
        if (result.file_extension == ext) {
            score += 15.0;
            break;
        }
    }

    // 4. Prefer moderate file sizes.
    if (result.file_size < 50 * 1024)
        score += 10.0;
    else if (result.file_size < 500 * 1024)
        score += 5.0;
    else
        score -= 5.0;

    return score;
}