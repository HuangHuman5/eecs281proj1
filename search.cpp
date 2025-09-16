// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68251FC9
#include "search.h"
#include "changes.h"
#include "dictionary.h"

#include <deque>
#include <vector>
#include <string>
#include <algorithm>

using std::vector;
using std::string;
using std::deque;
using std::size_t;

SearchResult search(const Dictionary &dict, size_t begin, size_t end, Strategy strat, const modtype &ops) {
    const vector<string> &W = dict.words();
    const size_t n = W.size();

    vector<char> discovered(n, 0);
    vector<size_t> parent(n, 0);
    vector<char> hasParent(n, 0);

    deque<size_t> sc;
    sc.push_back(begin);
    discovered[begin] = 1;
    hasParent[begin] = 0; 

    size_t discoveredCount = 1;
    bool found = false;

    while (!sc.empty()) {
        size_t pos;
        if (strat == Strategy::Queue) {
            pos = sc.front();
            sc.pop_front();
        } else {
            pos = *(sc.end()-1);
            sc.pop_back();
        }

        const string &cw = W[pos];
        for (size_t i = 0; i < n; i++) {
            if (discovered[i]) 
                continue;
            const string &cand = W[i];
            if (!ops.length && cand.size() != cw.size()) 
                continue;
            if (ops.length && (cand.size() + 1 < cw.size() || cand.size() > cw.size() + 1)) 
                continue;
            if (edit(cw, cand, ops)) {
                discovered[i] = 1;
                parent[i] = pos;
                hasParent[i] = 1;
                discoveredCount++;
                sc.push_back(i);

                if (i == end) {
                    found = true;
                    break; 
                }
            }
        }
       
        if (found) 
            break;

    }

    SearchResult res;
    res.discovered = discoveredCount;
    res.found = found;

    if (found) {
        vector<size_t> rev;
        size_t v = end;
        while (true) {
            rev.push_back(v);
            if (!hasParent[v]) 
                break;
            v = parent[v];
        }
        std::reverse(rev.begin(), rev.end());
        res.path = std::move(rev);
    }
    return res;
}