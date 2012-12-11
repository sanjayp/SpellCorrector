/*
 * File: spelling_manager.cpp
 * Author: Sanjay Paul
 *
 * Desc: The main manager for the spelling correction facility. Handles all
 * relevant memory objects and caching. 
 */

#include "spelling_manager.h"

using namespace std;

void SpellingManager::initializeDictionary(string fileLoc) {
   // Prepare dictionary file for reading
   ifstream stream;
   stream.open(fileLoc.c_str()); 
   if (!stream || !stream.is_open()) {
       cerr << "Invalid dictionary file location." << endl;
       exit(-1);
   }
   
   // Generate dictionary
   string word;
   while (stream >> word) {
    to_lower(word);
    dictionary.insert(word); 
   }

   // Generate lowercase letter list
   for (int i = 0; i < 26; i++) {
    string letter;
    letter.push_back((char)(i+97));
    letters.push_back(letter);
   }
}

bool SpellingManager::isValidWord(string word) const{
    string temp(word);
    // Trim out non-alphanumeric characters from edges
    trim_if(temp,!(is_from_range('a','z') || is_from_range('A','Z') || is_digit()));
    // Lowercase the word
    to_lower(temp);
    return dictionary.count(temp);
}

lst SpellingManager::getCorrections(string word) const{
    lst output;

    // Generate valid edit distance 1 words
    pair<lst,set<string>> result = generateEdits(word, 1);
    set<string> corrections = result.second;
    set<string>::iterator it;
    for (it=corrections.begin();it!=corrections.end();it++) {
        output.push_back(*it);
    }

    // Generate valid edit distance 2 words
    lst edits1 = result.first;
    for (int i = 0; i < edits1.size(); i++) {
        pair<lst,set<string>> tmp = generateEdits(edits1[i],0);
        for (it=tmp.second.begin();it!=tmp.second.end();it++) {
            if (corrections.insert(*it).second) {
                output.push_back(*it);
            }
        }
    }

    //cout << word << " : "; 
    //for (int i = 0; i < output.size(); i++) {
       // cout << output[i] << ", ";
    //}
    //cout << endl << endl;

    return output;
}

pair<lst,set<string>> SpellingManager::generateEdits(string word, bool return_edits) const{
    lst edits;
    set<string> corrections;

    // Generate deletions
    for (int i = 0; i < word.length(); i++) {
        string modified(word);
        modified.erase(i,1);
        if (return_edits)
            edits.push_back(modified);
        if (isValidWord(modified)){
            corrections.insert(modified);
        }
    }

    // Generate replacements (alphabetic only)
    for (int i = 0; i < word.length(); i++) {
        for (int j = 0; j < letters.size(); j++) {
            string modified(word);
            modified.replace(i,1,letters[j]);
            if (return_edits)
                edits.push_back(modified);
            if (isValidWord(modified)) {
                corrections.insert(modified);
            }
        }
    }

    // Generate insertions (alphabetic only)
    for (int i = 0; i < word.length()+1; i++) {
        for (int j = 0; j < 26; j++) {
            string modified(word);
            modified.insert(i,letters[j]);
            if (return_edits)
                edits.push_back(modified);
            if (isValidWord(modified)) {
                corrections.insert(modified);
            }
        }
    }

    // Generate transpositions
    for (int i = 1; i < word.length(); i++) {
        string modified(word);
        string before = word.substr(i-1,1);
        modified.replace(i-1,1,word.substr(i,1));
        modified.replace(i,1,before);
        if (return_edits)
            edits.push_back(modified);
        if (isValidWord(modified)) {
            corrections.insert(modified);
        }
    }

    if (corrections.count(word)>0)
        corrections.erase(word);

    pair <lst, set<string>> result (edits,corrections);
    return result;
}
