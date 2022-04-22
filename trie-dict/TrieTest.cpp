/*
Kevin Matsui
CS3505
A3: A Trie and Rule-of-Three
*/
#include <fstream>
#include <iostream>
#include <cstring>
#include "Trie.h"
using namespace std;

int main(int argc, char **argv) {
    if(argc < 2) {
        cout << "Enter 2 valid filenames.";
        return 0;
    }
    Trie testTree;
    char inputFile[256];
    char queryFile[256];
    strcpy(inputFile, argv[1]);
    strcpy(queryFile, argv[2]);
    string line;    
    ifstream input_file(inputFile);

    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << inputFile << "'" << endl;
        return EXIT_FAILURE;
    }

    while (std::getline(input_file, line)){
        // add words from the file to the tree
        if(line.length() >= 1) {
            testTree.addAWord(line);
        }
    }
    input_file.close();

    ifstream query_file(queryFile);
    if (!query_file.is_open()) {
        cerr << "Could not open the file - '"
             << queryFile << "'" << endl;
        return EXIT_FAILURE;
    }

    while(std::getline(query_file, line)) {
        // if word exists in tree print that its found
        if(testTree.isAWord(line)) {
            cout << line << " is found" << endl;
        }
        else {
            cout << line << " is not found, did you mean:" << endl;
            vector<string> prefixedWords = testTree.allWordsStartingWithPrefix(line);
            // if the word is not found, either print the alternatives, or no alternatives found
            if(prefixedWords.size() != 0) {
                for (vector<string>::iterator it=prefixedWords.begin(); it!=prefixedWords.end(); ++it) {
                     cout<< "   " << *it << endl;
                }
            }
            else {
                cout << "   no alternatives found" << endl;
            }
        }
    }
    query_file.close();
}