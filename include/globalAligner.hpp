#pragma once
#include <string>
#include <vector>

struct AlignmentResult
{
    int finalScore;
    std::vector<std::vector<int>> scoreMatrix;
    std::vector<std::pair<std::string, std::string>> alignments;
};

class GlobalAligner
{
private:
    int matchScore;
    int mismatchPenalty;
    int gapPenalty;

    void initializeMatrix(std::vector<std::vector<int>> &dp, int m, int n) const;
    void fillMatrix(std::vector<std::vector<int>> &dp, const std::string &s1, const std::string &s2, int m, int n) const;
    std::vector<std::pair<std::string, std::string>> backtrackAlignments(
        const std::vector<std::vector<int>> &dp,
        const std::string &s1, const std::string &s2,
        int m, int n) const;
    struct Aligned
    {
        std::vector<char> a1, a2;
    };
    void dfs(const std::vector<std::vector<int>> &dp,
             const std::string &seq1, const std::string &seq2,
             int i, int j, Aligned &aligned,
             std::vector<std::pair<std::string, std::string>> &alignments) const;

public:
    GlobalAligner(int match = 1, int mismatch = -1, int gap = -2);
    AlignmentResult align(const std::string &seq1, const std::string &seq2);
};