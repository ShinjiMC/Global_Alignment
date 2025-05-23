#include "alignmentIO.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

std::pair<std::string, std::string> AlignmentIO::loadSequences(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Could not open file: " + filename);
    std::string line1, line2;
    if (!std::getline(file, line1) || !std::getline(file, line2))
        throw std::runtime_error("Error reading lines from file: " + filename);
    return {line1, line2};
}

void AlignmentIO::writeToFile(const std::string &filename,
                              int score,
                              const std::vector<std::vector<int>> &matrix,
                              const std::vector<std::pair<std::string, std::string>> &alignments)
{
    std::ofstream out(filename);
    if (!out)
    {
        std::cerr << "Could not open file: " << filename << "\n";
        return;
    }
    out << "Score: " << score << "\n\n";
    out << "Matrix:\n";
    for (const auto &row : matrix)
    {
        for (int val : row)
            out << val << "\t";
        out << "\n";
    }
    out << "\nQuantity of alignments generated: "
        << alignments.size() << "\n\n";
    out << "Alignments generated:\n";
    for (const auto &[a1, a2] : alignments)
        out << a1 << "\n"
            << a2 << "\n\n";
}
