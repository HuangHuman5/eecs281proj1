// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68251FC9
#include "dictionary.h"
#include "changes.h"
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::size_t;

void Dictionary::addWord(const string &w) {
    if (index.find(w) == index.end()) {
        index[w] = wordlist.size();
        wordlist.push_back(w);
    }
}

void Dictionary::getwords(const std::string &begin, const std::string &end, const modtype &ops) {
    wordlist.clear();
    index.clear();
    
    string lineinput;
    if (!(std::cin >> lineinput)) 
        throw std::runtime_error("No input");
    lineinput = shorten(lineinput);
    if (lineinput != "S" && lineinput != "C") 
        throw std::runtime_error("First line should be 'S' or 'C'");
    string nline;
    if (!(std::cin >> nline)) 
        throw std::runtime_error("Missing Number of Letters Indicator");
    
    nline = shorten(nline);

    int N = 0;
    try { 
        N = std::stoi(nline); 
    } 
    catch (...) { 
        throw std::runtime_error("Invalid N"); 
    }
    if (N < 0) 
        throw std::runtime_error("Negative N");
    if (lineinput == "S") {
        index.reserve(N);
        wordlist.reserve(N);
    }
    string line;
    int count = 0;
    while (count < N && std::cin >> line) {
        if (line.find("//") == 0) {
            std::getline(std::cin, line);
            continue;
        }
        line = shorten(line);
        if (line.empty()) 
            return;
        if (lineinput == "S") {
            if (!isWord(line)) 
                throw std::runtime_error("Not in simple dictionary");
            if(!ops.length && line.size() != begin.size() && line.size() != end.size())
                continue;
            addWord(line);
        }
        else {
            vector<string> out;
            compLine(line, out);
            for (auto &w : out) {
                if (!isWord(w)) 
                    throw std::runtime_error("Not in complex dictionary");
                if(!ops.length && line.size() != begin.size() && line.size() != end.size())
                    continue;
                addWord(w);
            }
        }
        count++;
    }
}


size_t Dictionary::indexOf(const std::string &w) const {
    auto it = index.find(w);
    if (it == index.end()) 
        throw std::out_of_range("Word not in dictionary: " + w);
    return it->second;
}

bool Dictionary::contains(const std::string &w, size_t &Idx) const {
    auto it = index.find(w);
    if (it == index.end()) 
        return false;
    Idx = it->second;
    return true;
}

