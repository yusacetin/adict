/*
This file is part of Adict.

Adict is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Adict is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Adict. If not, see <https://www.gnu.org/licenses/>.
*/

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