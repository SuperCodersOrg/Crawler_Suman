#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <sstream>
#include "../../DS_Library/include/dynamicArray.h"

using std::string;

class Tokenizer
{
public:

    DynamicArray<string> tokenize(DynamicArray<string>& text);

private:

    bool isAlphabet(char ch);

    bool isDigit(char ch);

    bool isPunctuation(char ch);

    char toLowerCase(char ch);

    string normalizeWord(const string& word);

    bool isNumeric(const string& word);
};

#endif