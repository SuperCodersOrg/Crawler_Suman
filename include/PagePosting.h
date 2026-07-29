#ifndef PAGEPOSTING_H
#define PAGEPOSTING_H

#include <string>

using std::string;

class PagePosting{
public:
    string word;
    int frequency;
    PagePosting();
    PagePosting(const string& word,int frequency);
};

#endif