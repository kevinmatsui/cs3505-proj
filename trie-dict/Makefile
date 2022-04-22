CC = g++ 
CFLAGS = -Wall -std=c++11 -fsanitize=address -fsanitize=undefined

TrieTest : Node.o Trie.o TrieTest.o
	$(CC) $(CFLAGS) -g -o TrieTest TrieTest.o Trie.o Node.o

TrieTest.o : TrieTest.cpp Trie.h
	$(CC) $(CFLAGS) -c -g TrieTest.cpp Trie.cpp

Trie.o : Trie.h Trie.cpp
	$(CC) $(CFLAGS) -c -g Trie.cpp

Node.o : Node.h Node.cpp
	$(CC) $(CFLAGS) -c -g Node.cpp

clean : 
	rm TrieTest *.o

test :
	./TrieTest input.txt query.txt