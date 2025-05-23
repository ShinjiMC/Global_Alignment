#include "scoreCalculator.hpp"
#include <algorithm>

int ScoreCalculator::calculateScore(const std::string &a, const std::string &b) const
{
    int score = 0;
    size_t len = std::min(a.size(), b.size());
    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] == b[i])
            score += same;
        else
            score += diff;
    }
    return score;
}