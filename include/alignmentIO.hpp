#pragma once
#include <string>
#include <vector>
#include <utility>
#include <string>

class AlignmentIO
{
public:
    static std::pair<std::string, std::string> loadSequences(const std::string &filename);
    static void writeToFile(const std::string &filename,
                            int score,
                            const std::vector<std::vector<int>> &matrix,
                            const std::vector<std::pair<std::string, std::string>> &alignments);
};
