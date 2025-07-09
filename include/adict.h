/*
This file is part of Adict.

Adict is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Adict is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Adict. If not, see <https://www.gnu.org/licenses/>.
*/

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
    std::map<std::string, std::string> meta;
    std::map<std::string, std::string> style;

    // Object functions
    void print();
    DOCX compile();

    // Static functions
    static Adict read(std::string fpath);
};

#endif