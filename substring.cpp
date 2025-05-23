#include <iostream>
#include "alignmentIO.hpp"
#include "substringChecker.hpp"

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
        if (SubstringChecker::isSubstring(s1, s2))
            std::cout << "String 2 is a substring of String 1.\n";
        else if (SubstringChecker::isSubstring(s2, s1))
            std::cout << "String 1 is a substring of String 2.\n";
        else
            std::cout << "Neither string is a substring of the other.\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
