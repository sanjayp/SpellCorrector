/*
 * File: spelling_manager.cpp
 * Author: Sanjay Paul
 *
 * Desc: The main manager for the spelling correction facility. Handles all
 * relevant memory objects and caching. 
 */

#include "spelling_manager.h"

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

    // Step #2: Generate edit distance 2 on word and check validity
}
 
list SpellingManager::generateEdits1(string word) {
}

list SpellingManager::generateEdits2(list edits1) {
}
