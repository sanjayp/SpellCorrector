/*
 * =====================================================================================
 *
 *       Filename:  spell_corrector_test.cpp
 *
 *    Description:  Extensive testing for the spell_corrector, based on
 *    benchmarks stored in resources directory (location: '../resources/benchmarks').
 *    Should only be run inside the original project (uses relative paths to
 *    retrieve persistent data).
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

string root_path; // root prefix path of project

/*
 * Runs the checker over a newline-separated text file of words.
 * Reports the percentage word inclusion to stdout. Returns the
 * number of words not captured by the dictionary.
 */
int testChecker(SpellingManager* mgr, string filename) {
   
   // Open file to be checked
   ifstream stream;
   stream.open(filename);
   if (!stream || !stream.is_open()) {
       cerr << "Invalid location" << filename << endl;
       exit(-1);
   }

   string line;
   lst words;
   int correct = 0;
   int checked = 0;

   // Process the words in file
   while(stream >> line) {
    split(words, line, is_space());
    checked+=words.size();
    for (int i = 0; i < words.size(); i++){
        if(mgr->isValidWord(words[i])) {
            correct++;
        }
    }
   }

   // Report to stdout
   split(words, filename, is_any_of("/"));
   cout << "File: " << words.back() <<". Found " << correct; 
   cout << " valid words out of " << checked << ". Inclusion is "; 
   cout << 100*(((double)correct)/((double)checked)) << "%" << endl;

   return (checked - correct);
}

/*
 * Processes a benchmark file and logs failed corrections to a file
 * in '../resources/benchmarks/logs'. The output file name is
 * timestamped.
 *
 * NOTE: Works only with benchmark files with entries formatted as 
 * '<correct_word> : <incorrect spelling #1>, <incorrect spelling #2>, 
 * ...' with newline separation between entries.
 */
void testCorrectorOnBenchmark(SpellingManager* mgr, string filename) {
    
    // Prepare the benchmark file for reading
    ifstream stream(filename);
    if (!stream || !stream.is_open()) {
        cerr << "Invalid location" << filename << endl;
        exit(-1);
    }
    
    // Prepare output log file for writing
    stringstream ss;
    ss << time(0);
    string output_loc = root_path.append("resources/benchmarks/logs/");
    output_loc.append(ss.str());
    output_loc.append(".log");
    ofstream output(output_loc);

    cout << "Now running benchmark. This should run several hours - please be extremely patient." << endl;

    time_t start = time(0);

    // Proceed line-by-line through benchmark file and process.
    string line;
    stringstream buf;
    int corrected_count = 0;
    int checked = 0;
    int not_found = 0;
    long correction_time = 0;
    while(getline(stream,line)) {

        // Parse and split line
        lst parsed;
        split(parsed,line,(is_any_of(":,")));
        lst wrong;
        for (int i = 1; i < parsed.size(); i++) {
            wrong.push_back(parsed[i]);
        }
        string correct = parsed[0];
        trim(correct);

        // Generate corrections
        for (int i=0; i < wrong.size(); i++) {
            checked++;
            string check(wrong[i]);
            trim(check);
            time_t start_correct = time(0);
            lst suggested = mgr->getCorrections(check);
            correction_time+ = time(0) - start_correct; 
            if (find(suggested.begin(),suggested.end(),correct)!=suggested.end()) {
                corrected_count++;
            } else {
                buf << "Failed. Checked '" << check << "' and expected '" << correct << "' ";
                if (!(mgr->isValidWord(correct))) {
                    buf << " (NOTE: expected word not in dictionary)";
                    not_found++;
                }
                buf << endl;
            }
        }
        cout << "\rProcessed : " << checked << " |  Rate (wps): " << (double)checked/(double)correction_time << " | Accuracy: " << ((double)corrected_count/(double)checked)*100 << "% | Uptime (mins): " << (time(0)-start)/60 << "             " << flush;
    }

    // Record statistics to log file and dump correction failure buffer.
    long duration = time(0) - start;
    lst words;
    split(words, filename, is_any_of("/"));
    output << "Benchmark: " << words.back() << endl;
    output << "Total Runtime: " << duration << "s" << endl;
    output << "Incorrect Words Checked: " << checked << endl;
    output << "Correction Accuracy: " << (((double)corrected_count)/((double)checked))*100 << "% " << "(" << corrected_count << ")" << endl; 
    output << "Average Correction Speed: " << ((double)checked/(double)duration) << " wps" << endl; 
    output << "Dictionary Accuracy: " << ((double)(checked - not_found)/(double)checked)*100 << "% (" << not_found << " words not present)" << endl << endl;
    output << "Failure Log: " << endl << endl;
    output << buf.str();

    cout << "\rCompleted " << checked << " corrections in " << duration << " seconds. Log file recorded to " << output_loc << endl;
}

int main(int argc, char* argv[]) {
    
    // Retrieve the project root path
    char* buf = new char[1024];
    readlink("/proc/self/exe", buf, 1024);
    string current_path(buf);
    lst pieces;
    split(pieces, current_path, is_any_of("/"));
    for(int i = 0; i < pieces.size() - 2; i++) {
        root_path.append(pieces[i]);
        root_path.append("/");
    }
    cout << "Project root directory: " << root_path << endl << endl;

    // Create the manager
    SpellingManager* mgr = new SpellingManager();
    mgr->initializeDictionary(root_path + "resources/dictionaries/master.dic");

    // Run Tests
    
    // Test Checker Against Master Dictionary (100%)
    testChecker(mgr,root_path + "resources/dictionaries/master.dic");
    // Test Checker Against Texts
    testChecker(mgr,root_path + "resources/texts/prince.txt");
    // Test Checker Against Benchmark Words
    testChecker(mgr,root_path + "resources/dictionaries/benchmark_words/spell-errors.dic");
    // Test Benchmark
    testCorrectorOnBenchmark(mgr, root_path + "resources/benchmarks/spell-errors.txt");

    cout << "Finished testing. Quitting now." << endl;
    return 0;
}
