/*
 * =====================================================================================
 *
 *       Filename:  spell_corrector_test.cpp
 *
 *    Description:  Extensive testing for the spell_corrector, based on
 *    benchmarks stored in resources directory (location: '../resources/benchmarks').
 *
 *        Version:  1.0
 *        Created:  12/04/2012 07:52:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sanjay Paul 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "spelling_manager.h"
#include <sstream>
#include <ctime>

/*
 * Runs the checker over a newline-separated text file of words.
 * Reports the percentage word inclusion to stdout.
 */
void testChecker(SpellingManager* mgr, string filename) {
   ifstream stream;

   stream.open(filename);
  
   if (!stream || !stream.is_open()) {
       cerr << "Invalid location." << endl;
       exit(-1);
   }

   string line;
   lst words;
   int correct = 0, checked = 0;

   while(stream >> line) {
    split(words, line, is_space());
    checked+=words.size();
    for (int i = 0; i < words.size(); i++){
        if(mgr->isValidWord(words[i])) {
            correct++;
        }
    }
   }
   split(words, filename, is_any_of("/"));
   cout << "File: " << words.back() <<". Found " << correct << " valid words out of " << checked << ". Inclusion is " << 100*(((double)correct)/((double)checked)) << "%" << endl;
}

/*
 * Processes a benchmark file and logs failed corrections to a file
 * in '../resources/benchmarks/logs'.
 *
 * NOTE: Works only with benchmark files formatted as '<correct_word> :
 * <incorrect spelling #1>, <incorrect spelling #2>, ...'
 */
void testCorrectorOnBenchmark(SpellingManager* mgr, string filename) {
    ifstream stream(filename);

    if (!stream || !stream.is_open()) {
        cerr << "Invalid location." << endl;
        exit(-1);
    }
    
    time_t curr = time(0);
    stringstream ss;
    ss << curr;
    string output_loc = "/home/sp/final190/project/resources/benchmarks/logs/";
    output_loc.append(ss.str());
    output_loc.append(".log");
    ofstream output(output_loc);

    string line;
    int corrected_count = 0, checked = 0;
    while(getline(stream,line)) {
        trim(line);
        lst parsed;
        split(parsed,line,(is_any_of(":,")));
        lst wrong(parsed.begin()+1,parsed.end());
        string correct = parsed[0];
        for (int i=0; i < wrong.size(); i++) {
            checked++;
            string check(wrong[i]);
            trim(check);
            lst suggested = mgr->getCorrections(check);
            if (find(suggested.begin(),suggested.end(),correct)!=suggested.end()) {
                corrected_count++;
                break;
            } else {
                output << "Failed: " << check << ". Expected: " << correct << endl;
            }
        }
    }
    output << "Checked " << checked << " words. Correction Accuracy: " << (((double)corrected_count)/((double)checked))*100 << "% " << "(" << corrected_count << ")." << endl; 
}

int main(int argc, char* argv[]) {
    if(argc!=2) {
        cerr << "Please input, as a single argument, the file location "
            << "of dictionary to be used. Exiting." << endl;
        return -1;
    }

    SpellingManager* mgr = new SpellingManager();
    mgr->initializeDictionary(argv[1]);

    // Run Tests
    testChecker(mgr,"/home/sp/final190/project/resources/dictionaries/master.dic");
    testChecker(mgr,"/home/sp/final190/project/resources/texts/prince.txt");
    testChecker(mgr,"/home/sp/final190/project/resources/dictionaries/benchmark_words/spell-errors.dic");
    testCorrectorOnBenchmark(mgr, "/home/sp/final190/project/resources/benchmarks/spell-errors.txt");

    cout << "Finished testing. Quitting now." << endl;
    return 0;
}
