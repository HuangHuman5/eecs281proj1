// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68251FC9
#ifndef LETTERMAN_SEARCH_H
#define LETTERMAN_SEARCH_H

#include <string>
#include <vector>
#include "changes.h"
#include "dictionary.h"

struct SearchResult {
    bool found = false;
    std::vector<size_t> path;
    size_t discovered = 0;
};

enum class Strategy { Queue, Stack };

SearchResult search(const Dictionary &dict, size_t beginIdx,size_t endIdx,Strategy strat,const modtype &ops);

#endif // LETTERMAN_SEARCH_H
