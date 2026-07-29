#ifndef POSTING_H
#define POSTING_H
#include <iostream>
class Posting
{
public:

    int pageID;

    int frequency;

    Posting();

    Posting(
        int pageID,
        int frequency);

    bool operator==(
        const Posting& other) const;
};


#endif