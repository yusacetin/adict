#ifndef ADICT_H
#define ADICT_H

#include "word.h"
#include "../../docx/docx.hpp"

#include <string>
#include <vector>

class Adict {
public:
    // Data variables
    std::vector<Word> words;

    // Object functions
    void print();
    DOCX compile();

    // Static functions
    static Adict read(std::string fpath);
};

#endif