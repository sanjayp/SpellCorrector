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
#include <iostream>
#include <fstream>

using namespace std;

typedef unordered_set<string> table;
typedef vector<string> list;

class SpellingManager {
    private:
        table dictionary;
        list generateEdits1(string word);
        list generateEdits2(list edits1);
        table initializeDictionary(string fileLoc);

    public:
        bool isValidWord(string word) const;
        list getCorrections(string word) const;

};

#endif
