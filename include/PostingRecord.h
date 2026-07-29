#ifndef POSTINGRECORD_H
#define POSTINGRECORD_H

#include <string>

using std::string;

/*
    Represents one row of the postings table.

    Example:

    word      = "crawler"
    pageID    = 5
    frequency = 3

    Used only while transferring data
    between SQLite and the in-memory
    inverted index.
*/

class PostingRecord
{
public:

    string word;

    int pageID;

    int frequency;

    PostingRecord();

    PostingRecord(
        const string& word,
        int pageID,
        int frequency);
};

#endif