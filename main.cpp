/*
This file is part of Adict.

Adict is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Adict is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Adict. If not, see <https://www.gnu.org/licenses/>.
*/

#include "include/adict.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Please provide the adict JSON file path as an argument" << "\n";
        return 1;
    }

    if (!std::filesystem::exists(argv[1])) {
        std::cerr << "File does not exist: " << argv[1] << "\n";
        return 1;
    }

    Adict adict = Adict::read(argv[1]);
    adict.print();

    std::string oname;
    if (argc >= 3) {
        oname = argv[2];
    } else {
        std::string argv1 = argv[1];
        std::string final_fpath_no_json_ext = argv1;
        if (argv1.size() > 5) { // dot and 'json'
            std::string sub = final_fpath_no_json_ext.substr(final_fpath_no_json_ext.size() - 4, 4);
            if ((sub == "json") || (sub == "JSON")) {
                std::string noext = final_fpath_no_json_ext.substr(0, final_fpath_no_json_ext.size() - 5);
                final_fpath_no_json_ext = noext;
            }
        }
        oname = final_fpath_no_json_ext + ".docx";
    }

    adict.compile().save(oname);

    return 0;
}