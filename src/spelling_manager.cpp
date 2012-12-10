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

    // Generate lowercase letters as strings
    for (int i = 0; i < 26; i++) {
        string letter;
        letter.push_back((char)(i+97));
        letters.push_back(letter);
    }

    return dictionary;
}

bool SpellingManager::isValidWord(string word) const{
    string temp(word);
    trim_if(temp,!(is_from_range('a','z') || is_from_range('A','Z') || is_digit()));
    // Accounts for words at start of sentences. Consider removing
    // to its own method.
    string lower(temp);
    lower[0] = tolower(temp[0]);
    return dictionary.count(temp) + dictionary.count(lower) > 0;
}

lst SpellingManager::getCorrections(string word) const{
    // Generate edit distance 1 on word and check validity
    pair<lst,set<string>> result = generateEdits(word, 1);
    set<string> corrections = result.second;
    lst edits1 = result.first;
    for (int i = 0; i < edits1.size(); i++) {
        pair<lst,set<string>> tmp = generateEdits(edits1[i],0);
        set<string> tmpset = tmp.second;
        set<string>::iterator it;
        for (it=tmpset.begin();it!=tmpset.end();it++) {
            corrections.insert(*it);
        }
    }

    lst output(corrections.begin(),corrections.end());
    //cout << word << ": "; 
    for (int i = 0; i < output.size(); i++) {
      //  cout << output[i] << ", ";
    }
    //cout << endl;
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
        if (dictionary.count(modified)){
            corrections.insert(modified);
            //cout << word << " " << modified << endl;
        }
        // short-circuit if finished early
        if (corrections.size() >= MAX_CORRECTIONS) {
            break;
        }
    }

    // Generate replacements (alphabetic only)
    for (int i = 0; i < word.length(); i++) {
        for (int j = 0; j < letters.size(); j++) {
            string modified(word);
            modified.replace(i,1,letters[j]);
            if (return_edits)
                edits.push_back(modified);
            if (dictionary.count(modified)) {
                corrections.insert(modified);
                //cout << modified << endl;
            }
        }
        // short-circuit if finished early
        if (corrections.size() >= MAX_CORRECTIONS) {
            break;
        }
    }

    // Generate insertions (alphabetic only)
    for (int i = 0; i < word.length()+1; i++) {
        for (int j = 0; j < 26; j++) {
            string modified(word);
            modified.insert(i,letters[j]);
            if (return_edits)
                edits.push_back(modified);
            if (dictionary.count(modified)) {
                corrections.insert(modified);
                //cout << modified << endl;
            }
        }
        // short-circuit if finished early
        if (corrections.size() >= MAX_CORRECTIONS) {
            break;
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
        if (dictionary.count(modified) && modified.compare(word)) {
            corrections.insert(modified);
            //cout << word << " " << modified << endl;
        }
    }

    pair <lst, set<string>> result (edits,corrections);
    return result;
}
