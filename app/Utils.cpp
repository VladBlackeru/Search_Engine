//
// Created by Vlod on 05/11/2025.
//

#include "Utils.h"
#include <regex>

std::string replaceBackslashes(const std::string& input) {
    std::regex backslashPattern("\\\\");
    return std::regex_replace(input, backslashPattern, "/");
}