#include <gtest/gtest.h>
#include "substringChecker.hpp"
#include "scoreCalculator.hpp"
#include "globalAligner.hpp"

// ---------- SubstringChecker Tests ----------

TEST(SubstringCheckerTest, BasicCases)
{
    EXPECT_TRUE(SubstringChecker::isSubstring("CGAGGGGTDGATAGACGTA", "CGTA"));
    EXPECT_TRUE(SubstringChecker::isSubstring("CGTA", "CGTA"));
    EXPECT_FALSE(SubstringChecker::isSubstring("CGTA", "CGAGGGGTDGATAGACGTA"));
    EXPECT_TRUE(SubstringChecker::isSubstring("CGAGGGGTCGATAGACGTA", "GGTC"));
}

TEST(SubstringCheckerTest, ExactMatch)
{
    EXPECT_TRUE(SubstringChecker::isSubstring("A", "A"));
    EXPECT_FALSE(SubstringChecker::isSubstring("A", "T"));
    EXPECT_TRUE(SubstringChecker::isSubstring("ATCGATCG", "ATCGATCG"));
}

TEST(SubstringCheckerTest, EmptyStrings)
{
    EXPECT_TRUE(SubstringChecker::isSubstring("", ""));
    EXPECT_TRUE(SubstringChecker::isSubstring("AGTC", ""));
    EXPECT_FALSE(SubstringChecker::isSubstring("", "AGTC"));
}

TEST(SubstringCheckerTest, StartMiddleEnd)
{
    EXPECT_TRUE(SubstringChecker::isSubstring("GATTACA", "GAT"));
    EXPECT_TRUE(SubstringChecker::isSubstring("GATTACA", "TTA"));
    EXPECT_TRUE(SubstringChecker::isSubstring("GATTACA", "ACA"));
    EXPECT_FALSE(SubstringChecker::isSubstring("GATTACA", "GTCA"));
}

TEST(SubstringCheckerTest, LongerSubstrings)
{
    EXPECT_TRUE(SubstringChecker::isSubstring("TTGACGATCGGATCGA", "CGATCGGAT"));
    EXPECT_FALSE(SubstringChecker::isSubstring("TTGACGATCGGATCGA", "CGAAGG"));
    EXPECT_FALSE(SubstringChecker::isSubstring("ATATATAT", "ATATATATAT"));
}

// ---------- ScoreCalculator Tests ----------
TEST(ScoreCalculatorTest, MatchMismatchVariations)
{

    ScoreCalculator scorer(1, -2);
    EXPECT_EQ(scorer.calculateScore("ACCCGTAG", "ACCAGTTG"), 2);
    EXPECT_EQ(scorer.calculateScore("AAAA", "AAAA"), 4);
    EXPECT_EQ(scorer.calculateScore("AAAA", "TTTT"), -8);
    EXPECT_EQ(scorer.calculateScore("ACGT", "ACGT"), 4);

    ScoreCalculator scorer1(2, -1);
    EXPECT_EQ(scorer1.calculateScore("ACGT", "ACGT"), 8);  // All match
    EXPECT_EQ(scorer1.calculateScore("AAAA", "TTTT"), -4); // All mismatch
    EXPECT_EQ(scorer1.calculateScore("AGCT", "AGTT"), 5);  // 3 match, 1 mismatch

    ScoreCalculator scorer2(3, -2);
    EXPECT_EQ(scorer2.calculateScore("GGGG", "GGGA"), 7);        // 3 match, 1 mismatch
    EXPECT_EQ(scorer2.calculateScore("CCCC", "CCCC"), 12);       // 4 match
    EXPECT_EQ(scorer2.calculateScore("GATTACA", "GCTTACC"), 11); // 4 match, 3 mismatch

    ScoreCalculator scorer3(1, -3);
    EXPECT_EQ(scorer3.calculateScore("ACGT", "TGCA"), -12); // All mismatch
    EXPECT_EQ(scorer3.calculateScore("AAAA", "AAAT"), 0);   // 3 match, 1 mismatch
    EXPECT_EQ(scorer3.calculateScore("TTTT", "TTTT"), 4);   // All match
}

TEST(ScoreCalculatorTest, EdgeCases)
{
    ScoreCalculator scorer(2, -2);

    EXPECT_EQ(scorer.calculateScore("", ""), 0);    // All empty
    EXPECT_EQ(scorer.calculateScore("A", ""), 0);   // One empty
    EXPECT_EQ(scorer.calculateScore("", "A"), 0);   // One empty
    EXPECT_EQ(scorer.calculateScore("A", "T"), -2); // One mismatch
    EXPECT_EQ(scorer.calculateScore("A", "A"), 2);  // One match
}

// ---------- GlobalAligner Tests ----------

TEST(GlobalAlignerTest, RealCase1)
{
    GlobalAligner aligner(1, -1, -2);
    std::string s1 = "TACCGAT";
    std::string s2 = "ATACCATACGT";
    auto [score, matrix, alignments] = aligner.align(s1, s2);

    EXPECT_EQ(score, -3);
    EXPECT_EQ(alignments.size(), 2);

    std::vector<std::string> expectedAligned1 = {"-TACCG-A--T", "-TACC-GA--T"};
    std::string expectedAligned2 = "ATACCATACGT";

    bool match1 = (alignments[0].second == expectedAligned2 || alignments[1].second == expectedAligned2);
    EXPECT_TRUE(match1);

    bool foundAll = true;
    for (const auto &expected : expectedAligned1)
    {
        bool found = false;
        for (const auto &pair : alignments)
            if (pair.first == expected && pair.second == expectedAligned2)
                found = true;
        if (!found)
            foundAll = false;
    }
    EXPECT_TRUE(foundAll);
}

TEST(GlobalAlignerTest, RealCase2)
{
    GlobalAligner aligner(1, -1, -2);
    std::string s1 = "ACGT";
    std::string s2 = "ACGT";
    auto [score, matrix, alignments] = aligner.align(s1, s2);

    EXPECT_EQ(score, 4);
    EXPECT_EQ(alignments.size(), 1);

    std::string expectedAligned1 = "ACGT";
    std::string expectedAligned2 = "ACGT";

    EXPECT_EQ(alignments[0].first, expectedAligned1);
    EXPECT_EQ(alignments[0].second, expectedAligned2);
}

TEST(GlobalAlignerTest, IdenticalSequences)
{
    GlobalAligner aligner(2, -1, -2);
    std::string s1 = "GATTACA";
    std::string s2 = "GATTACA";
    auto [score, matrix, alignments] = aligner.align(s1, s2);

    EXPECT_EQ(score, 14);
    ASSERT_EQ(alignments.size(), 1);
    EXPECT_EQ(alignments[0].first, "GATTACA");
    EXPECT_EQ(alignments[0].second, "GATTACA");
}

TEST(GlobalAlignerTest, CompletelyDifferent)
{
    GlobalAligner aligner(1, -1, -2);
    std::string s1 = "AAAA";
    std::string s2 = "TTTT";
    auto [score, matrix, alignments] = aligner.align(s1, s2);

    EXPECT_EQ(score, -4);
    ASSERT_EQ(alignments.size(), 1);
    EXPECT_EQ(alignments[0].first, "AAAA");
    EXPECT_EQ(alignments[0].second, "TTTT");
}

TEST(GlobalAlignerTest, WithGaps)
{
    GlobalAligner aligner(1, -1, -2);
    std::string s1 = "ACT";
    std::string s2 = "ACGT";
    auto [score, matrix, alignments] = aligner.align(s1, s2);

    EXPECT_EQ(score, 1);
    EXPECT_GE(alignments.size(), 1);
    for (const auto &[a1, a2] : alignments)
    {
        EXPECT_EQ(a1.length(), a2.length());
        for (char c : a1)
            EXPECT_TRUE(c == 'A' || c == 'C' || c == 'T' || c == '-' || c == 'G');
    }
}

TEST(GlobalAlignerTest, EmptySequence)
{
    GlobalAligner aligner(1, -1, -2);
    std::string s1 = "";
    std::string s2 = "ACGT";
    auto [score, matrix, alignments] = aligner.align(s1, s2);

    EXPECT_EQ(score, -8);
    ASSERT_EQ(alignments.size(), 1);
    EXPECT_EQ(alignments[0].first, "----");
    EXPECT_EQ(alignments[0].second, "ACGT");
}

TEST(GlobalAlignerTest, TieMultipleAlignments)
{
    GlobalAligner aligner(1, -1, -2);
    std::string s1 = "GGAAG";
    std::string s2 = "GAAA";
    auto [score, matrix, alignments] = aligner.align(s1, s2);

    EXPECT_EQ(score, 0);
    EXPECT_GE(alignments.size(), 2);

    std::set<std::pair<std::string, std::string>> expected = {
        {"GGAAG", "-GAAA"},
        {"GGAAG", "G-AAA"},
        {"GGAAG", "GAAA-"}};

    for (const auto &[a1, a2] : alignments)
        EXPECT_EQ(a1.length(), a2.length());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}