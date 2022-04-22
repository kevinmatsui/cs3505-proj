/*
Kevin Matsui
CS3505
A3: A Trie and Rule-of-Three
*/
#include "Trie.h"
using std::string;
using std::vector;

// used node's default constructor for root
Trie::Trie() : root() {
}

// adds given lowercase word to the Trie
void Trie::addAWord(string word) {
    Node* curNode= &root;
    int ascii;
    // used to convert ascii to array index
    int offset = 97;
    for(int i = 0; i < word.size(); i++) {
        // get ascii value of the character
        ascii = int(word[i]);
        // if the position of the current letter in the branches array is null, then make a new node
        if(curNode->branches[ascii-offset]== nullptr) {          
            curNode->branches[ascii-offset] = new Node();
            curNode = curNode->branches[ascii-offset];
        }
        else{         
            curNode = curNode->branches[ascii-offset];
        }
    }
    curNode->isWord = true;
}

// returns true if the exact given word exists in the trie
bool Trie::isAWord(string word) {
    Node* curNode = &root; 
    int ascii;
    // used to convert ascii to array index
    int offset = 97;
    if(word.length() < 1) {
        return false;
    }
    for(int i = 0; i < word.length(); i++) {
        ascii = int(word[i]);
        if(curNode->branches[ascii-offset]) {
            curNode = curNode->branches[ascii-offset];
        }
        else {
            return false;
        }
    }
    return curNode->isWord;
}
// returns a vector of all strings that begin with the prefix
vector<string> Trie::allWordsStartingWithPrefix(string prefix) {
    vector<string> allWords;
    Node* curNode = &root; 
    string newPrefix;
    int ascii;
    // used to convert ascii to array index
    int offset = 97;
    for(int i = 0; i < prefix.size(); i++) {
        ascii = int(prefix[i]);
        // if branch for given letter is not null, continue to that branch
        if(curNode->branches[ascii-offset]) {
            curNode = curNode->branches[ascii-offset];
        }
        else {
            return allWords;
        }
    }
    if(curNode->isWord) {
        allWords.push_back(prefix);
    }
    for(int i = 0; i < 26; i++) {
        if(curNode->branches[i]) {
            newPrefix = prefix + char(i+offset);
            // recursively call this method on the word with the added character, and add the result vector to allWords
            vector<string> allWordsNewPrefix = allWordsStartingWithPrefix(newPrefix);
            allWords.insert(allWords.end(), allWordsNewPrefix.begin(), allWordsNewPrefix.end());
        }
    }
    return allWords;
}