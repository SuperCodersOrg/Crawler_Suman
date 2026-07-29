#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <string>

#include "../../DS_Library/include/DynamicArray.h"
#include "../../DS_Library/include/HashMap.h"

#include "Posting.h"

using std::string;
class InvertedIndex{
public:
    void insert(const string& word,int pageID,int frequency);
    bool contains(const string& word);
    DynamicArray<Posting>& getPostings(const string& word);
    int vocabularySize();
    void clear();

private:
    HashMap<string, DynamicArray<Posting>> index;
};

#endif