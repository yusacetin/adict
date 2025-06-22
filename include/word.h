#ifndef WORD_H
#define WORD_H

#include <string>
#include <vector>

class Word {
public:
    std::string name;
    std::string definition;
    std::vector<std::string> etymology;
    std::vector<std::string> examples;
};

#endif