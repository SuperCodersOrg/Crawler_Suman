#ifndef INDEXSTORAGE_H
#define INDEXSTORAGE_H

#include <string>

#include "../../DS_Library/include/DynamicArray.h"

#include "Database.h"
#include "PagePosting.h"
#include "PostingRecord.h"
#include "InvertedIndex.h"

using std::string;

class IndexStorage
{
private:
    Database database;
public:
    IndexStorage();
    bool initialize(const string& databaseName);
    bool savePage(int pageID,DynamicArray<PagePosting>& pagePostings);
    int load(InvertedIndex& index);
    void clear();
};

#endif