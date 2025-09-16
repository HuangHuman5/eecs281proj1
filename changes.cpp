// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68251FC9
#include "changes.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::min;
using std::max;

std::string shorten(const std::string &s) {
    size_t i = 0, j = s.size();
    while (i < j && s[i] == ' ') 
        i++;
    while (j > i && s[j - 1] == ' ') 
        j--;
    return s.substr(i, j - i);
}

bool isWord(const std::string &s) {
    if (s.empty()) 
        return false;
    for (char c : s) 
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) 
            return false;
    return true;
}


static void reverse(std::string word, std::vector<std::string> &out) {
    if (!word.empty() && *(word.end()-1) == '&') 
        word.pop_back();
    out.push_back(word);
    std::string rev = word;
    std::reverse(rev.begin(), rev.end());
    out.push_back(rev);
}

static void insertEach(const std::string &line, size_t left, size_t right, std::vector<std::string> &out) {
    std::string before = line.substr(0, left);
    std::string inside = line.substr(left + 1, right - left - 1);
    std::string after = line.substr(right + 1);
    for (char c : inside) {
        std::string w = before + c + after;
        out.push_back(w);
    }
}

static void swaptwo(const std::string &line, size_t pos, std::vector<std::string> &out) {
    std::string base = line;
    if (pos < base.size()) 
        base.erase(pos, 1);
    out.push_back(base);
    if (pos >= 2) {
        std::string sw = base;
        std::swap(sw[pos-2], sw[pos-1]);
        out.push_back(sw);
    }
}

static void doubleQ(const std::string &line, size_t pos, std::vector<std::string> &out) {
    std::string base = line;
    if (pos < base.size()) 
        base.erase(pos, 1);
    out.push_back(base);
    if (pos >= 1) {
        std::string d = base;
        d.insert(d.begin() + (pos - 1), base[pos-1]);
        out.push_back(d);
    }
}

void compLine(const std::string &lineIn, std::vector<std::string> &out) {
    std::string line = shorten(lineIn);
    if (line.empty()) 
        return;

    size_t amp = line.find('&');
    size_t lb = line.find('[');
    size_t rb = line.find(']');
    size_t bang = line.find('!');
    size_t ques = line.find('?');

    if (amp != string::npos) {
        reverse(line, out);
        return;
    }
    if (lb != string::npos && rb != string::npos && rb > lb + 1) {
        insertEach(line, lb, rb, out);
        return;
    }
    if (bang != string::npos) {
        swaptwo(line, bang, out);
        return;
    }
    if (ques != string::npos) {
        doubleQ(line, ques, out);
        return;
    }
    out.push_back(line);
}


static bool change(const string &a, const string &b) {
    if (a.size() != b.size()) return false;
    int diff = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            diff++;
            if (diff > 1) 
                return false;
        }
    }
    return diff == 1;
}

static bool insert(const string &shorter, const string &longer) {
    if (longer.size() != shorter.size() + 1) return false;
    size_t i = 0, j = 0;
    while (i < shorter.size() && j < longer.size() && shorter[i] == longer[j]) {
         i++; j++; 
        }
    j++;
    while (i < shorter.size() && j < longer.size() && shorter[i] == longer[j]) {
        i++; j++;
    }
    return i == shorter.size() && j == longer.size();
}

static bool oneSwap(const string &a, const string &b) {
    if (a.size() != b.size() || a.size() < 2) 
        return false;
    size_t i = 0;
    while (i < a.size() && a[i] == b[i]){
        i++;
    }
    if (i >= a.size())
        return false;
    if (i + 1 >= a.size())
        return false;
    if (a[i] == b[i+1] && a[i+1] == b[i]){
        for (size_t k = i + 2; k < a.size(); k++){
            if (a[k] != b[k])
                return false;
        }
        return true;
    }
    return false;
}

bool edit(const std::string &a, const std::string &b, const modtype &ops) {
    if (a == b) 
        return false;
    bool ok = false;
    if (!ok && ops.change) 
        ok = (change(a, b));
    if (!ok && ops.length) {
        if (a.size() + 1 == b.size()) 
            ok = (insert(a, b));
        else if (a.size() == b.size() + 1) 
            ok = (insert(b, a));
    }
    if (!ok && ops.swap) 
        ok = (oneSwap(a, b));
    return ok;
}

Modification computeMod(const std::string &a, const std::string &b) {
    Modification m;
    m.op = 'c';
    m.pos = 0;
    m.ch = '\0';

    if (a.size() == b.size()) {
        size_t i = 0;
        while (i < a.size() && a[i] == b[i]){ 
            i++;
        }
        if (i < a.size() && i + 1 < a.size() && a[i] == b[i+1] && a[i+1] == b[i]) {
            m.op = 's';
            m.pos = i;
            return m;
        }
        if (i < a.size()) {
            m.op = 'c';
            m.pos = i;
            m.ch = b[i];
            return m;
        }
        throw std::logic_error("computeModification is called on identical strings");
    }
    else if (a.size() + 1 == b.size()) {
        size_t i = 0, j = 0;
        while (i < a.size() && j < b.size() && a[i] == b[j]) {
            i++; j++;
        }
        m.op = 'i';
        m.pos = j;
        m.ch = b[j];
        return m;
    }
    else if (a.size() == b.size() + 1) {
        size_t i = 0, j = 0;
        while (i < b.size() && j < a.size() && a[j] == b[i]) {
            i++; j++;
        }
        m.op = 'd';
        m.pos = j;
        return m;
    }

    throw std::logic_error("computeModification is called on a non-1-edit pair");
}
