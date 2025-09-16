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

void Dictionary::getwords() {
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
            addWord(line);
        }
        else {
            vector<string> out;
            compLine(line, out);
            for (auto &w : out) {
                if (!isWord(w)) 
                    throw std::runtime_error("Not in complex dictionary");
                addWord(w);
            }
        }
        count++;
    }
}

void Dictionary::filter(const std::string &begin, const std::string &end, const modtype &ops) {
    std::vector<std::string> newList;
    std::unordered_map<std::string, size_t> newIndex;

    size_t startLen = begin.size();

    for (const auto &w : wordlist) {
        size_t len = w.size();
        bool keep = false;
        if (ops.swap && !ops.change && !ops.length) {
            if (len == startLen) {
                keep = true;
                for(const char &c: w) {
                if(begin.find(c) != std::string::npos) {
                    keep = false;
                    break;
                }
            }
            }
        }
        if (!ops.length) {
            if (len != startLen) {
                keep = false;
            }
        }
        if (w == begin || w == end) {
            keep = true;
        }
        if (keep) {
            newIndex[w] = newList.size();
            newList.push_back(w);
        }
    }
    wordlist.swap(newList);
    index.swap(newIndex);
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

