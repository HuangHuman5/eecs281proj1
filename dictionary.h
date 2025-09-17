// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68251FC9
#ifndef LETTERMAN_DICTIONARY_H
#define LETTERMAN_DICTIONARY_H

#include <string>
#include <vector>
#include <unordered_map>
#include "changes.h"

class Dictionary {
private:
    std::vector<std::string> wordlist;
    std::unordered_map<std::string, size_t> index;

    void addWord(const std::string &w);

public:
    void getwords(const std::string &begin, const std::string &end, const modtype &ops);

    const std::vector<std::string>& words() const {
        return wordlist;
    }

    size_t size() const {
        return wordlist.size(); 
    }

    size_t indexOf(const std::string &w) const;

    bool contains(const std::string &w, size_t &outIdx) const;
    
    void filter(const std::string &begin, const std::string &end, const modtype &ops) ;
};

#endif // LETTERMAN_DICTIONARY_H
