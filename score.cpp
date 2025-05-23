#include <iostream>
#include "alignmentIO.hpp"
#include "scoreCalculator.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Use: " << argv[0] << " <path_txt>\n";
        return 1;
    }
    std::string filename = argv[1];
    try
    {
        auto [s1, s2] = AlignmentIO::loadSequences(filename);
        ScoreCalculator scorer(1, -2);
        int score = scorer.calculateScore(s1, s2);
        std::cout << "Score: " << score << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
