#include "../include/PostingRecord.h"

PostingRecord::PostingRecord()
{
    pageID = 0;
    frequency = 0;
}

PostingRecord::PostingRecord(
    const string& word,
    int pageID,
    int frequency)
{
    this->word = word;
    this->pageID = pageID;
    this->frequency = frequency;
}