#pragma once
#include <string>

class ScoreCalculator
{
private:
    int same, diff;

public:
    ScoreCalculator(int same = 1, int diff = -2) : same(same), diff(diff) {}
    int calculateScore(const std::string &a, const std::string &b) const;
};
