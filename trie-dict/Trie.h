/*
Kevin Matsui
CS3505
A3: A Trie and Rule-of-Three
*/
#ifndef TRIE_H
#define TRIE_H
#include "Node.h"
#include <string>
#include <vector>

class Trie{
    Node root;
    public:
    Trie();
    void addAWord(std::string word);
    bool isAWord(std::string word);
    std::vector<std::string> allWordsStartingWithPrefix(std::string prefix);
};
#endif