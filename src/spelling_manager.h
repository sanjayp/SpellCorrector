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
#include <algorithm>
//#include <boost/algorithm/string.hpp>

using namespace std;
//using namespace boost;

typedef unordered_set<string> table;
typedef vector<string> lst;

class SpellingManager {
    public:
        // Initializes the dictionary from persistent dictionary file.
        void initializeDictionary(string fileLoc);
        // Checks validity of a word. NOTE: handles trimming and
        // camel-case option for all checks
        bool isValidWord(string word) const;
        // Produces list of possible corrections for the provided word
        // (up to MAX_CORRECTIONS in total).
        lst getCorrections(string word) const;

    private:
        table dictionary;
        lst letters;
        string delimiter;
        pair<lst,set<string>> generateEdits(string word, bool return_edits) const;
};

#endif
