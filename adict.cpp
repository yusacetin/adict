/*
This file is part of Adict.

Adict is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Adict is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Adict. If not, see <https://www.gnu.org/licenses/>.
*/

// Program includes
#include "include/adict.h"
#include "include/global_definitions.h"

// Library include
#include "include/json.hpp"
using json = nlohmann::json;

// Standard includes
#include <iostream>
#include <fstream>
#include <algorithm> // for sorting words alphabetically

// Methods

Adict Adict::read(std::string fpath) {
    std::ifstream f(fpath);
    json dict_json = json::parse(f);
    Adict adict;
    for (size_t i=0; i<dict_json["words"].size(); i++) {
        Word word;

        word.name = dict_json["words"][i]["name"];
        word.definition = dict_json["words"][i]["definition"];
        
        if (dict_json["words"][i].contains("etymology")) {
            if (dict_json["words"][i]["etymology"].is_array()) {
                for (size_t e_i=0; e_i<dict_json["words"][i]["etymology"].size(); e_i++) {
                    word.etymology.push_back(dict_json["words"][i]["etymology"][e_i]);
                }
            } else {
                word.etymology.push_back(dict_json["words"][i]["etymology"]);
            }
        }
        
        if (dict_json["words"][i].contains("examples")) {
            if (dict_json["words"][i]["examples"].is_array()) {
                for (size_t e_i=0; e_i<dict_json["words"][i]["examples"].size(); e_i++) {
                    word.examples.push_back(dict_json["words"][i]["examples"][e_i]);
                }
            } else {
                word.examples.push_back(dict_json["words"][i]["examples"]);
            }
        }
        
        adict.words.push_back(word);
    }
    return adict;
}

void Adict::print() {
    // Sort words alphabetically first
    std::sort(words.begin(), words.end(), [](const Word& a, const Word& b) {
        return a.name < b.name;
    });

    for (size_t i = 0; i < words.size(); i++) {
        Word w = words.at(i);
        std::cout << "* " << w.name << ": " << w.definition << newl;

        if (w.etymology.size() > 0) {
            std::cout << "+> etym.: ";
            for (size_t i=0; i<w.etymology.size(); i++) {
                std::cout << w.etymology.at(i);
                if (i != w.etymology.size()-1) {
                    std::cout << " + ";
                }
            }
            std::cout << newl;
        }
        
        if (w.examples.size() > 0) {
            std::cout << "+> examples: ";
            for (size_t i=0; i<w.examples.size(); i++) {
                std::cout << w.examples.at(i);
                if (i != w.examples.size()-1) {
                    std::cout << ", ";
                }
            }
            std::cout << newl;
        }

        if (i < words.size()-1) {
            std::cout << newl;
        }
    }
}

DOCX Adict::compile() {
    // Sort words alphabetically first
    std::sort(words.begin(), words.end(), [](const Word& a, const Word& b) {
        return a.name < b.name;
    });

    DOCX docx;
    for (size_t i = 0; i < words.size(); i++) {
        Word cur_word = words.at(i);

        // Fist line (name and definition)
        DOCX::Paragraph p;

        p.add_bold_text(cur_word.name);
        p.add_bold_text(":");
        p.add_space();
        p.add_plain_text(cur_word.definition);

        docx.add_paragraph(p);

        size_t global_font_size = docx.get_global_font_size();
        size_t subsize = global_font_size - 1;
        if (global_font_size <= 1) {
            subsize = 1;
        }

        // Second line (etymology)
        if (!cur_word.etymology.empty()) {
            DOCX::Paragraph p2;

            DOCX::Text etym_label("etym.");
            etym_label.size = subsize;
            etym_label.italic = true;
            p2.add_text(etym_label);

            DOCX::Text etym_colon(":");
            etym_colon.size = subsize;
            p2.add_text(etym_colon);

            p2.add_space(1, subsize);

            for (size_t e_i = 0; e_i < cur_word.etymology.size(); e_i++) {
                DOCX::Text etym_content(cur_word.etymology.at(e_i));
                etym_content.size = subsize;
                p2.add_text(etym_content);

                if (e_i < cur_word.etymology.size()-1) {
                    DOCX::Text comma(",");
                    comma.size = subsize;
                    p2.add_text(comma);
                    p2.add_space(1, subsize);
                }
            }

            docx.add_paragraph(p2);
        }

        // Third line (examples)
        if (!cur_word.examples.empty()) {
            DOCX::Paragraph p3;

            DOCX::Text exs_label("ex.s.");
            exs_label.size = subsize;
            exs_label.italic = true;
            p3.add_text(exs_label);

            DOCX::Text exs_colon(":");
            exs_colon.size = subsize;
            p3.add_text(exs_colon);

            p3.add_space(1, subsize);

            for (size_t e_i = 0; e_i < cur_word.examples.size(); e_i++) {
                DOCX::Text quote("\"");
                quote.size = subsize;
                p3.add_text(quote);

                DOCX::Text exs_content(cur_word.examples.at(e_i));
                exs_content.size = subsize;
                p3.add_text(exs_content);

                p3.add_text(quote);

                if (e_i < cur_word.examples.size()-1) {
                    DOCX::Text comma(",");
                    comma.size = subsize;
                    p3.add_text(comma);
                    p3.add_space(1, subsize);
                }
            }

            docx.add_paragraph(p3);
        }

        if (i < words.size()-1) {
            docx.add_empty_line();
        }
    }
    return docx;
}