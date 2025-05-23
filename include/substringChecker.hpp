#pragma once
#include <string>
#include <vector>

class SubstringChecker
{
public:
    static std::vector<int> buildPrefixTable(const std::string &pattern);
    static bool isSubstring(const std::string &text, const std::string &pattern);
};
