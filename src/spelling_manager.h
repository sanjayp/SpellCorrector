/*
 * File: spelling_manager.h
 * Author: Sanjay Paul
 *
 * Desc: The manager for the spelling correction facility. Offers instantiation 
 * methods and a main method for testing.
 */

#ifndef SPELLINGMANAGER_H_
#define SPELLINGMANAGER_H_

#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#define MAX_CORRECTIONS 20

using namespace std;
using namespace boost;

typedef unordered_set<string> table;
typedef vector<string> lst;

class SpellingManager {
    public:
        table initializeDictionary(string fileLoc);
        bool isValidWord(string word) const;
        lst getCorrections(string word) const;

    private:
        table dictionary;
        lst letters;
        pair<lst,set<string>> generateEdits(string word, bool return_edits) const;
};

#endif
