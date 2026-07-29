#include "StopWordRemover.h"

StopWordRemover::StopWordRemover()
{
    initializeStopWords();
}

void StopWordRemover::initializeStopWords()
{
    //--------------------------------------------------
    // Articles
    //--------------------------------------------------
    stopWords.insert("a", true);
    stopWords.insert("an", true);
    stopWords.insert("the", true);

    //--------------------------------------------------
    // Be Verbs
    //--------------------------------------------------
    stopWords.insert("am", true);
    stopWords.insert("is", true);
    stopWords.insert("are", true);
    stopWords.insert("was", true);
    stopWords.insert("were", true);
    stopWords.insert("be", true);
    stopWords.insert("been", true);
    stopWords.insert("being", true);

    //--------------------------------------------------
    // Auxiliary Verbs
    //--------------------------------------------------
    stopWords.insert("have", true);
    stopWords.insert("has", true);
    stopWords.insert("had", true);

    stopWords.insert("do", true);
    stopWords.insert("does", true);
    stopWords.insert("did", true);

    //--------------------------------------------------
    // Modal Verbs
    //--------------------------------------------------
    stopWords.insert("will", true);
    stopWords.insert("would", true);
    stopWords.insert("shall", true);
    stopWords.insert("should", true);
    stopWords.insert("can", true);
    stopWords.insert("could", true);
    stopWords.insert("may", true);
    stopWords.insert("might", true);
    stopWords.insert("must", true);

    //--------------------------------------------------
    // Conjunctions
    //--------------------------------------------------
    stopWords.insert("and", true);
    stopWords.insert("or", true);
    stopWords.insert("but", true);
    stopWords.insert("if", true);
    stopWords.insert("because", true);
    stopWords.insert("so", true);

    //--------------------------------------------------
    // Prepositions
    //--------------------------------------------------
    stopWords.insert("of", true);
    stopWords.insert("to", true);
    stopWords.insert("in", true);
    stopWords.insert("on", true);
    stopWords.insert("at", true);
    stopWords.insert("by", true);
    stopWords.insert("for", true);
    stopWords.insert("from", true);
    stopWords.insert("with", true);
    stopWords.insert("into", true);
    stopWords.insert("over", true);
    stopWords.insert("under", true);
    stopWords.insert("between", true);
    stopWords.insert("through", true);
    stopWords.insert("during", true);
    stopWords.insert("before", true);
    stopWords.insert("after", true);

    //--------------------------------------------------
    // Demonstratives
    //--------------------------------------------------
    stopWords.insert("this", true);
    stopWords.insert("that", true);
    stopWords.insert("these", true);
    stopWords.insert("those", true);

    //--------------------------------------------------
    // Pronouns
    //--------------------------------------------------
    stopWords.insert("i", true);
    stopWords.insert("me", true);
    stopWords.insert("my", true);

    stopWords.insert("you", true);
    stopWords.insert("your", true);

    stopWords.insert("he", true);
    stopWords.insert("him", true);
    stopWords.insert("his", true);

    stopWords.insert("she", true);
    stopWords.insert("her", true);

    stopWords.insert("it", true);
    stopWords.insert("its", true);

    stopWords.insert("we", true);
    stopWords.insert("our", true);
    stopWords.insert("us", true);

    stopWords.insert("they", true);
    stopWords.insert("them", true);
    stopWords.insert("their", true);

    //--------------------------------------------------
    // Miscellaneous
    //--------------------------------------------------
    stopWords.insert("as", true);
    stopWords.insert("not", true);
    stopWords.insert("no", true);
    stopWords.insert("here", true);
    stopWords.insert("there", true);
    stopWords.insert("very", true);
    stopWords.insert("too", true);
    stopWords.insert("also", true);
}

DynamicArray<string> StopWordRemover::removeStopWords(DynamicArray<string>& tokens){
    DynamicArray<string> filteredTokens;
    for(int i = 0; i < tokens.getSize(); i++){
        if(tokens[i].empty()){
            continue;
        }
        if(!stopWords.exists(tokens[i])){
            filteredTokens.push_back(tokens[i]);
        }
    }
    return filteredTokens;
}