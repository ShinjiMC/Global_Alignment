#include "globalAligner.hpp"
#include <algorithm>
#include <functional>

GlobalAligner::GlobalAligner(int match, int mismatch, int gap)
    : matchScore(match), mismatchPenalty(mismatch), gapPenalty(gap) {}

AlignmentResult GlobalAligner::align(const std::string &seq1, const std::string &seq2)
{
    int m = seq1.size(), n = seq2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    initializeMatrix(dp, m, n);
    fillMatrix(dp, seq1, seq2, m, n);
    auto alignments = backtrackAlignments(dp, seq1, seq2, m, n);
    int finalScore = dp[m][n];
    return {finalScore, dp, alignments};
}

void GlobalAligner::initializeMatrix(std::vector<std::vector<int>> &dp, int m, int n) const
{
    for (int i = 0; i <= m; ++i)
        dp[i][0] = i * gapPenalty;
    for (int j = 0; j <= n; ++j)
        dp[0][j] = j * gapPenalty;
}

void GlobalAligner::fillMatrix(std::vector<std::vector<int>> &dp,
                               const std::string &seq1, const std::string &seq2,
                               int m, int n) const
{
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
        {
            int scoreDiag = dp[i - 1][j - 1] + ((seq1[i - 1] == seq2[j - 1]) ? matchScore : mismatchPenalty);
            int scoreUp = dp[i - 1][j] + gapPenalty;
            int scoreLeft = dp[i][j - 1] + gapPenalty;
            dp[i][j] = std::max({scoreDiag, scoreUp, scoreLeft});
        }
}

void GlobalAligner::dfs(const std::vector<std::vector<int>> &dp,
                        const std::string &seq1, const std::string &seq2,
                        int i, int j, Aligned &aligned,
                        std::vector<std::pair<std::string, std::string>> &alignments) const
{
    if (i == 0 && j == 0)
    {
        std::string s1(aligned.a1.rbegin(), aligned.a1.rend());
        std::string s2(aligned.a2.rbegin(), aligned.a2.rend());
        alignments.emplace_back(s1, s2);
        return;
    }

    if (i > 0 && dp[i][j] == dp[i - 1][j] + gapPenalty)
    {
        aligned.a1.push_back(seq1[i - 1]);
        aligned.a2.push_back('-');
        dfs(dp, seq1, seq2, i - 1, j, aligned, alignments);
        aligned.a1.pop_back();
        aligned.a2.pop_back();
    }
    if (j > 0 && dp[i][j] == dp[i][j - 1] + gapPenalty)
    {
        aligned.a1.push_back('-');
        aligned.a2.push_back(seq2[j - 1]);
        dfs(dp, seq1, seq2, i, j - 1, aligned, alignments);
        aligned.a1.pop_back();
        aligned.a2.pop_back();
    }
    if (i > 0 && j > 0)
    {
        int score = (seq1[i - 1] == seq2[j - 1]) ? matchScore : mismatchPenalty;
        if (dp[i][j] == dp[i - 1][j - 1] + score)
        {
            aligned.a1.push_back(seq1[i - 1]);
            aligned.a2.push_back(seq2[j - 1]);
            dfs(dp, seq1, seq2, i - 1, j - 1, aligned, alignments);
            aligned.a1.pop_back();
            aligned.a2.pop_back();
        }
    }
}

std::vector<std::pair<std::string, std::string>>
GlobalAligner::backtrackAlignments(const std::vector<std::vector<int>> &dp,
                                   const std::string &seq1, const std::string &seq2,
                                   int m, int n) const
{
    std::vector<std::pair<std::string, std::string>> alignments;
    Aligned initial;
    dfs(dp, seq1, seq2, m, n, initial, alignments);
    return alignments;
}