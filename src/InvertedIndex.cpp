#include "../include/InvertedIndex.h"
void InvertedIndex::insert(const string& word,int pageID,int frequency){
    if(!index.exists(word)){
        DynamicArray<Posting> postings;
        postings.push_back(Posting(pageID,frequency));
        index.insert(word,postings);
        return;
    }
    DynamicArray<Posting>& postings = index.get(word);
    for(int i = 0;i < postings.getSize();i++){
        if(postings[i].pageID == pageID){
            postings[i].frequency = frequency;
            return;
        }
    }
    postings.push_back(Posting(pageID,frequency));
}

bool InvertedIndex::contains(const string& word){
    return index.exists(word);
}

DynamicArray<Posting>&InvertedIndex::getPostings(const string& word){
    return index.get(word);
}

int InvertedIndex::vocabularySize(){
    return index.getSize();
}

void InvertedIndex::clear(){
    index.clear();
}