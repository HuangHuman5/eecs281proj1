// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68251FC9
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>


#include "dictionary.h"
#include "changes.h"
#include "search.h"


using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;


static void printHelp() {
    cout << "Letterman Reboot — EECS 281\n"
    << "Usage: ./letter [--stack|-s | --queue|-q] --begin|-b WORD --end|-e WORD "
    "[--output|-o W|M] [--change|-c] [--length|-l] [--swap|-p] [--help|-h]\n";
}


static void die(const string &msg) {
    cerr << msg << endl;
    std::exit(1);
}


int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);


    bool StackOption = false, QueueOption = false;
    bool haveBegin = false, haveEnd = false;
    string beginWord, endWord;
    modtype ops;
    char outputMode = 'W';


    const option longopts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"stack", no_argument, nullptr, 's'},
        {"queue", no_argument, nullptr, 'q'},
        {"begin", required_argument, nullptr, 'b'},
        {"end", required_argument, nullptr, 'e'},
        {"output", required_argument, nullptr, 'o'},
        {"change", no_argument, nullptr, 'c'},
        {"length", no_argument, nullptr, 'l'},
        {"swap", no_argument, nullptr, 'p'},
        {nullptr, 0, nullptr, 0 }
    };

    int c;
    while ((c = getopt_long(argc, argv, "hsqb:e:o:clp", longopts, nullptr)) != -1) {
        switch (c) {
            case 'h': printHelp(); return 0;
            case 's': if (StackOption) die("Duplicate --stack"); StackOption = true; break;
            case 'q': if (QueueOption) die("Duplicate --queue"); QueueOption = true; break;
            case 'b': haveBegin = true; beginWord = optarg; break;
            case 'e': haveEnd = true; endWord = optarg; break;
            case 'o': {
                if (!optarg || (optarg[0] != 'W' && optarg[0] != 'M') || optarg[1] != '\0') {
                    die("Invalid --output (use W or M)");
                }
                outputMode = optarg[0];
                break;
            }
            case 'c': ops.change = true; break;
            case 'l': ops.length = true; break;
            case 'p': ops.swap = true; break;
            default: die("Unknown option");
        }
    }


    if (StackOption == QueueOption) 
        die("Only one of --stack or --queue");
    if (!haveBegin || !haveEnd) 
        die("Both --begin and --end are required");
    if (!ops.change && !ops.length && !ops.swap) 
        die("Specify at least one of --change, --length, --swap");


    Dictionary dict;
    try { 
        dict.getwords(); 
    }
    catch (const std::exception &ex) {
        die(string("Invalid dictionary: ") + ex.what()); 
    }

    size_t startIdx, endIdx;
    try {
        startIdx = dict.indexOf(beginWord);
        endIdx = dict.indexOf(endWord);
    }
    catch (const std::out_of_range&) {
        die("Starting/ending word not found in dictionary");
    }

    if (!ops.length && (beginWord.size() != endWord.size())) {
        die("Lengths differ but --length not indicated");
    }


    Strategy strat;
    if(QueueOption)
        strat = Strategy::Queue;
    else
        strat = Strategy::Stack;

    SearchResult res = search(dict, startIdx, endIdx, strat, ops);

    if (!res.found) {
        cout << "No solution, " << res.discovered << " words discovered." << '\n';
        return 0;
    }


    if (outputMode == 'W') {
        cout << "Words in morph: " << res.path.size() << '\n';
        for (size_t idx : res.path) cout << dict.words()[idx] << '\n';
    } 
    else {
        cout << "Words in morph: " << res.path.size() << '\n';
        cout << dict.words()[res.path.front()] << '\n';
        for (size_t i = 1; i < res.path.size(); ++i) {
            const string &prev = dict.words()[res.path[i-1]];
            const string &cur = dict.words()[res.path[i]];
            Modification m = computeMod(prev, cur);
            if (m.op == 'c' || m.op == 'i') {
                cout << m.op << ',' << m.pos << ',' << m.ch << '\n';
            } 
            else {
                cout << m.op << ',' << m.pos << '\n';
            }
        }
    }


    return 0;
}
