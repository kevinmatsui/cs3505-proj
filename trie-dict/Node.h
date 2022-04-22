/*
Kevin Matsui
CS3505
A3: A Trie and Rule-of-Three
*/
#ifndef NODE_H
#define NODE_H
class Node{
    public:
    Node* branches[26];
    bool isWord;
    Node();
    ~Node();
    Node(const Node& other);
    Node& operator=(Node other);
};
#endif