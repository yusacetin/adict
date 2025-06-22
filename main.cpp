#include "include/adict.h"

int main() {
    Adict adict = Adict::read("dict.json");
    adict.print();
    adict.compile().save("my_dictionary.docx");

    return 0;
}