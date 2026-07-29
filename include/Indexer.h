#ifndef INDEXER_H
#define INDEXER_H

#include <string>

#include "../../DS_Library/include/DynamicArray.h"

#include "PageStorage.h"
#include "HTMLParser.h"
#include "Tokenizer.h"
#include "StopWordRemover.h"
#include "InvertedIndex.h"
#include "IndexStorage.h"
#include "PagePosting.h"

using std::string;

class Indexer{
public:
    Indexer();
    void buildIndex();
    DynamicArray<string>search(const string& word);

private:
    bool initialize(const string& databaseName);
    DynamicArray<PagePosting> buildPagePostings(const string& html);
    int findPosting(DynamicArray<PagePosting>& postings,const string& word);

private:
    PageStorage pageStorage;
    HTMLParser parser;
    Tokenizer tokenizer;
    StopWordRemover remover;
    InvertedIndex index;
    IndexStorage storage;
    int lastIndexedPage;
};

#endif