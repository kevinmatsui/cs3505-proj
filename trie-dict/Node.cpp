/*
Kevin Matsui
CS3505
A3: A Trie and Rule-of-Three
*/
#include <iostream>
#include "Node.h"

// initializes the branches of the node to nullptrs
Node::Node() : branches() {
    isWord = false;
}
// deletes all of the branches, which recursively deletes all objects created
Node::~Node(){
    for(int i = 0; i < 26; i++) {
        delete branches[i];
    }
}
// creates a copy of another node
Node::Node(const Node& other) {
    isWord = other.isWord;
    for(int i = 0; i < 26; i++) {
        branches[i] = nullptr;
        if(other.branches[i]) {
            branches[i] = new Node(*(other.branches[i]));  
        }
    }
}
// assigns node to another node object
Node& Node::operator=(Node other) {
    std::swap(isWord, other.isWord);
    for(int i = 0; i < 26; i++) {
        std::swap(branches[i], other.branches[i]);
    }
    return *this;
}

