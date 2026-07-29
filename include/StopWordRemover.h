#ifndef STOPWORDREMOVER_H
#define STOPWORDREMOVER_H

#include <string>

#include "../../DS_Library/include/dynamicArray.h"
#include "../../DS_Library/include/hashMap.h"

using std::string;

class StopWordRemover
{
public:

    StopWordRemover();

    DynamicArray<string> removeStopWords(DynamicArray<string>& tokens);

private:

    HashMap<string,bool> stopWords;

    void initializeStopWords();
};

#endif