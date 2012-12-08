/*
 * File: spelling_manager.cpp
 * Author: Sanjay Paul
 *
 * Desc: The main manager for the spelling correction facility. Handles all
 * relevant memory objects and caching. 
 */

#include "spelling_manager.h"

#define MAX_CORRECTIONS 10;

using namespace std;

table SpellingManager::initializeDictionary(string fileLoc) {
   ifstream stream;
   stream.open(fileLoc.c_str()); 

   if (!stream || !stream.is_open()) {
       cerr << "Invalid dictionary file location." << endl;
       exit(-1);
   }
   
   string word;
   while (stream >> word) {
    dictionary.insert(word); 
   }

   return dictionary;
}

bool SpellingManager::isValidWord(string word) const{
    return dictionary.count(word);        
}

list SpellingManager::getCorrections(string word) const{
    // Step #1: Generate edit distance 1 on word and check validity
    list edits1;
    int corrections_found = 0;

    

    // Step #2: Generate edit distance 2 on word and check validity
}

list generateEdits(string word, int& correction_count) {

    list edits;
    list corrections;

    // Generate deletions
    for (int i = 0; i < word.length(); i++) {
        string modified = word.erase(i,1);
        edits.insert(modified);
        if (updateCount(correction_count,modified)) {
            corrections.insert(modified);
        }
        if ((*correction_count) >= MAX_CORRECTIONS) return edits;
    }
}

bool updateCount(int& count, string word) {
    if (isValidWord(word)) {
        (*count)++;
    }
    return (*count) >= MAX_CORRECTIONS;
}
