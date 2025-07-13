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

    if (dict_json.contains("meta")) {
        for (auto& [key, value] : dict_json["meta"].items()) {
            adict.meta[key] = value;
        }
    }

    if (dict_json.contains("style")) {
        for (auto& [key, value] : dict_json["style"].items()) {
            adict.style[key] = value;
        }
    }

    for (size_t i = 0; i < dict_json["words"].size(); i++) {
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

        if (dict_json["words"][i].contains("example_sentences")) {
            if (dict_json["words"][i]["example_sentences"].is_array()) {
                for (size_t e_i=0; e_i<dict_json["words"][i]["example_sentences"].size(); e_i++) {
                    word.example_sentences.push_back(dict_json["words"][i]["example_sentences"][e_i]);
                }
            } else {
                word.example_sentences.push_back(dict_json["words"][i]["example_sentences"]);
            }
        }

        if (dict_json["words"][i].contains("inspirations")) {
            if (dict_json["words"][i]["inspirations"].is_array()) {
                for (size_t e_i=0; e_i<dict_json["words"][i]["inspirations"].size(); e_i++) {
                    word.inspirations.push_back(dict_json["words"][i]["inspirations"][e_i]);
                }
            } else {
                word.inspirations.push_back(dict_json["words"][i]["inspirations"]);
            }
        }

        if (dict_json["words"][i].contains("notes")) {
            if (dict_json["words"][i]["notes"].is_array()) {
                for (size_t e_i=0; e_i<dict_json["words"][i]["notes"].size(); e_i++) {
                    word.notes.push_back(dict_json["words"][i]["notes"][e_i]);
                }
            } else {
                word.notes.push_back(dict_json["words"][i]["notes"]);
            }
        }
        
        adict.words.push_back(word);
    }
    return adict;
}

void Adict::print() {
    // Print meta
    bool meta_exists = false; // used to check if a space is necessary before the words section
    if (meta.find("title") != meta.end()) {
        std::cout << meta["title"] << newl;
        meta_exists = true;
    }

    if (meta.find("first_stroke") != meta.end()) {
        std::cout << "First Stroke: " << meta["first_stroke"] << newl;
        meta_exists = true;
    }

    if (meta_exists) {
        std::cout << newl;
    }

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

    std::cout << newl << "Number of words: " << words.size() << newl;
}

DOCX Adict::compile() {
    // Sort words alphabetically first
    std::sort(words.begin(), words.end(), [](const Word& a, const Word& b) {
        return a.name < b.name;
    });

    DOCX docx;

    bool meta_exists = false; // used to check if a space is necessary before the words section
    if (meta.find("title") != meta.end()) {
        DOCX::Paragraph title;
        DOCX::Text title_text(meta["title"]);

        if (style.find("title_size") != style.end()) {
            title_text.size = std::stoul(style["title_size"]);
        } else {
            title_text.size = 32;
        }

        title.add_text(title_text);
        title.align = DOCX::Paragraph::CENTER;
        docx.add_paragraph(title);
        meta_exists = true;
    }

    if (meta.find("first_stroke") != meta.end()) {
        DOCX::Paragraph first_stroke;
        DOCX::Text first_stroke_text("First Stroke: " + meta["first_stroke"]);
        
        if (style.find("first_stroke_size") != style.end()) {
            first_stroke_text.size = std::stoul(style["first_stroke_size"]);
        } else {
            first_stroke_text.size = 32;
        }

        first_stroke.add_text(first_stroke_text);
        first_stroke.align = DOCX::Paragraph::CENTER;
        docx.add_paragraph(first_stroke);
        meta_exists = true;
    }

    if (meta_exists) {
        docx.add_empty_line(2);
    }

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

            DOCX::Text exs_label("ex.");
            exs_label.size = subsize;
            exs_label.italic = true;
            p3.add_text(exs_label);

            DOCX::Text exs_colon(":");
            exs_colon.size = subsize;
            p3.add_text(exs_colon);

            p3.add_space(1, subsize);

            for (size_t e_i = 0; e_i < cur_word.examples.size(); e_i++) {
                DOCX::Text exs_content(cur_word.examples.at(e_i));
                exs_content.size = subsize;
                p3.add_text(exs_content);

                if (e_i < cur_word.examples.size()-1) {
                    DOCX::Text comma(",");
                    comma.size = subsize;
                    p3.add_text(comma);
                    p3.add_space(1, subsize);
                }
            }

            docx.add_paragraph(p3);
        }

        // Fourth line (example sentences)
        if (!cur_word.example_sentences.empty()) {
            DOCX::Paragraph p3;

            DOCX::Text exs_label("ex.s.");
            exs_label.size = subsize;
            exs_label.italic = true;
            p3.add_text(exs_label);

            DOCX::Text exs_colon(":");
            exs_colon.size = subsize;
            p3.add_text(exs_colon);

            p3.add_space(1, subsize);

            for (size_t e_i = 0; e_i < cur_word.example_sentences.size(); e_i++) {
                DOCX::Text quote("\"");
                quote.size = subsize;
                p3.add_text(quote);

                DOCX::Text exs_content(cur_word.example_sentences.at(e_i));
                exs_content.size = subsize;
                p3.add_text(exs_content);

                p3.add_text(quote);

                if (e_i < cur_word.example_sentences.size()-1) {
                    DOCX::Text comma(",");
                    comma.size = subsize;
                    p3.add_text(comma);
                    p3.add_space(1, subsize);
                }
            }

            docx.add_paragraph(p3);
        }

        // Fifth line (inspirations)
        if (!cur_word.inspirations.empty()) {
            DOCX::Paragraph p3;

            DOCX::Text insp_label("inspirations");
            insp_label.size = subsize;
            insp_label.italic = true;
            p3.add_text(insp_label);

            DOCX::Text insp_colon(":");
            insp_colon.size = subsize;
            p3.add_text(insp_colon);

            p3.add_space(1, subsize);

            for (size_t i_i = 0; i_i < cur_word.inspirations.size(); i_i++) {
                DOCX::Text insp_content(cur_word.inspirations.at(i_i));
                insp_content.size = subsize;
                p3.add_text(insp_content);

                if (i_i < cur_word.inspirations.size()-1) {
                    DOCX::Text comma(",");
                    comma.size = subsize;
                    p3.add_text(comma);
                    p3.add_space(1, subsize);
                }
            }

            docx.add_paragraph(p3);
        }

        // Sixth line onwards (notes)
        if (!cur_word.notes.empty()) {
            DOCX::Paragraph p3;

            DOCX::Text notes_label("notes");
            notes_label.size = subsize;
            notes_label.italic = true;
            p3.add_text(notes_label);

            DOCX::Text notes_colon(":");
            notes_colon.size = subsize;
            p3.add_text(notes_colon);

            p3.add_space(1, subsize);

            for (size_t n_i = 0; n_i < cur_word.notes.size(); n_i++) {
                DOCX::Text notes_content(cur_word.notes.at(n_i));
                notes_content.size = subsize;
                p3.add_text(notes_content);

                if (n_i < cur_word.notes.size()-1) {
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