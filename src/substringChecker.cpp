#include "substringChecker.hpp"
#include <vector>

std::vector<int> SubstringChecker::buildPrefixTable(const std::string &pattern)
{
    int m = pattern.size();
    std::vector<int> lps(m, 0);
    int len = 0;
    for (int i = 1; i < m;)
    {
        if (pattern[i] == pattern[len])
            lps[i++] = ++len;
        else if (len != 0)
            len = lps[len - 1];
        else
            lps[i++] = 0;
    }
    return lps;
}

bool SubstringChecker::isSubstring(const std::string &text, const std::string &pattern)
{
    if (pattern.empty())
        return true;
    if (text.empty())
        return false;
    int n = text.size();
    int m = pattern.size();
    std::vector<int> lps = buildPrefixTable(pattern);
    int i = 0, j = 0;
    while (i < n)
    {
        if (text[i] == pattern[j])
        {
            i++;
            j++;
            if (j == m)
                return true;
        }
        else if (j != 0)
            j = lps[j - 1];
        else
            i++;
    }
    return false;
}
