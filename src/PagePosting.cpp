#include "../include/PagePosting.h"

PagePosting::PagePosting()
{
    frequency = 0;
}

PagePosting::PagePosting(const string& word,int frequency)
{
    this->word = word;
    this->frequency = frequency;
}