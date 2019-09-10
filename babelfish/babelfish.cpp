/**
 * Date: 09/09/19
 * Author: Matthew M-T
 * Info:
 * Compiled and tested in g++ v9.2.1
 * 
 * Comments added 09/09/19, code finished 09/02/19
 */

#include <unordered_map>
#include <iostream>
#include <cassert>
#include <string>
#include <regex>

using namespace std;

const regex splitter("(\\S+)\\s+(\\S+)"); // Regex filter for output of getline();

void test();
void getInputs(unordered_map<string, string>& dictionary);
string checkWord(string& word, unordered_map<string, string>& dictionary);

/**
 * Main function of the program.
 */
int main(int argCount, char* args[]) {
    if (argCount > 1) {
        string arg = args[1];
        if (arg == "test") test(); // If the program is called from command line with an argument of "test", switch to the default test cases.
    } else {
        unordered_map<string, string> dictionary;
        getInputs(dictionary);
        string input;
        bool nl;
        while (cin >> input) {
            if (nl) cout << "\n" + checkWord(input, dictionary);
            else {
                nl = true;
                cout << checkWord(input, dictionary);
            }
        }
    }
}

/**
 * Function with default test cases for the checkWord() algorithm. Initializes an unordered_map of values and asserts proper output.
 */
void test() {
    unordered_map<string, string> assertion = {
        pair<string, string> {"histay", "this"},
        pair<string, string> {"ordway", "word"},
        pair<string, string> {"asway", "was"},
        pair<string, string> {"oodgay", "good"},
        pair<string, string> {"utbay", "but"},
        pair<string, string> {"eway", "we"},
        pair<string, string> {"hangedcay", "changed"},
        pair<string, string> {"tiay", "it"}
    };
    string word1 = "histay", word2 = "oodgay", word3 = "however";
    assert(checkWord(word1, assertion) == "this"); // assert 1
    assert(checkWord(word2, assertion) == "good"); // assert 2
    assert(checkWord(word3, assertion) == "eh");   // assert 3
    cout << "All test cases passed.";
}

/**
 * When getting input from a user/another program, this function parses user input and appends it to the given unordered_map.
 */
void getInputs(unordered_map<string, string>& dictionary) {
    string line;
    getline(cin, line);
    do {
        smatch returned;
        regex_match(line, returned, splitter);
        dictionary.insert(pair<string, string> (returned[2], returned[1]));
        getline(cin, line);
    } while (line != "");
    return;
}

/**
 * Returns the word corresponding to a keyword that is contained within the passed unordered_map, or returns "eh" if not found.
 */
string checkWord(string& word, unordered_map<string, string>& dictionary) {
    if (dictionary.count(word) > 0) return dictionary.at(word);
    else return "eh";
}