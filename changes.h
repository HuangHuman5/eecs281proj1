// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68251FC9
#ifndef LETTERMAN_UTILS_H
#define LETTERMAN_UTILS_H

#include <string>
#include <vector>


struct modtype {
    bool change = false;
    bool length = false;
    bool swap   = false;
};

struct Modification {
    char op; // 'c' change, 'i' insert, 'd' delete, 's' swap
    size_t  pos;
    char ch;
};

std::string shorten(const std::string &s);

bool isWord(const std::string &s);

void compLine(const std::string &line, std::vector<std::string> &out);

bool edit(const std::string &a, const std::string &b, const modtype &ops);

Modification computeMod(const std::string &a, const std::string &b);

#endif
