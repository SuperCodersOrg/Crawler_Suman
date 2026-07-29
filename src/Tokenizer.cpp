#include "Tokenizer.h"

using std::stringstream;

bool Tokenizer::isAlphabet(char ch){
    return (ch >= 'A' && ch <= 'Z') ||
           (ch >= 'a' && ch <= 'z');
}

bool Tokenizer::isDigit(char ch){
    return ch >= '0' && ch <= '9';
}

char Tokenizer::toLowerCase(char ch){
    if(ch >= 'A' && ch <= 'Z'){
        return ch + ('a' - 'A');
    }
    return ch;
}

bool Tokenizer::isPunctuation(char ch){
    return !(isAlphabet(ch) || isDigit(ch));
}

string Tokenizer::normalizeWord(const string& word){
    if(word.empty()){
        return "";
    }
    int start = 0;
    int end = word.length() - 1;
    while(start <= end && isPunctuation(word[start])){
        start++;
    }
    while(end >= start && isPunctuation(word[end])){
        end--;
    }
    if(start > end){
        return "";
    }
    string result;
    for(int i = start; i <= end; i++){
        result += toLowerCase(word[i]);
    }
    return result;
}

bool Tokenizer::isNumeric(const string& word){
    if(word.empty()){
        return false;
    }
    for(int i = 0; i < word.length(); i++){
        if(!isDigit(word[i])){
            return false;
        }
    }
    return true;
}

DynamicArray<string> Tokenizer::tokenize(DynamicArray<string>& text){
    DynamicArray<string> tokens;
    for(int i = 0; i < text.getSize(); i++){
        stringstream stream(text[i]);
        string word;
        while(stream >> word){
            word = normalizeWord(word);
            if(word.empty()){
                continue;
            }
            if(isNumeric(word)){
                continue;
            }
            tokens.push_back(word);
        }
    }
    return tokens;
}