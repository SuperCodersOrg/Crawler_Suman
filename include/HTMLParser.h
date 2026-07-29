#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <string>
#include "../../DS_Library/include/dynamicArray.h"

using std::string;

class HTMLParser
{
public:

    DynamicArray<string> extractText(const string& html);

private:

    bool isWhiteSpace(char ch);

    char toLowerCase(char ch);

    bool startsWith(const string& html,int position,const string& pattern);

    bool isTagBoundary(const string& html,int position);

    int findBodyStart(const string& html);

    string trim(const string& text);
};

#endif