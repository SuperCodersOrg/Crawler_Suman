#include "../include/Indexer.h"

#include <stdexcept>

bool Indexer::initialize(const string& databaseName){
    if(!storage.initialize(databaseName)){
        return false;
    }
    lastIndexedPage =storage.load(index);
    return true;
}

Indexer::Indexer() : lastIndexedPage(0){
    if(!storage.initialize("../CrawlerStorage/crawler.db")){
        throw std::runtime_error("Failed to initialize Indexer.");
    }
    lastIndexedPage = storage.load(index);
}

int Indexer::findPosting(DynamicArray<PagePosting>& postings,const string& word){
    for(int i = 0; i < postings.getSize(); i++){
        if(postings[i].word == word){
            return i;
        }
    }
    return -1;
}

DynamicArray<PagePosting>Indexer::buildPagePostings(const string& html){
    DynamicArray<PagePosting> pagePostings;
    DynamicArray<string> text = parser.extractText(html);
    DynamicArray<string> tokens = tokenizer.tokenize(text);
    DynamicArray<string> words =remover.removeStopWords(tokens);
    for(int i = 0; i < words.getSize(); i++){
        int position = findPosting(pagePostings,words[i]);
        if(position == -1){
            pagePostings.push_back(PagePosting(words[i],1));
        }
        else{
            pagePostings[position].frequency++;
        }
    }
    return pagePostings;
}

void Indexer::buildIndex(){
    int totalPages =pageStorage.pageCount();
    for(int pageID = lastIndexedPage + 1;pageID <= totalPages;pageID++){
        string url = pageStorage.getURLByID(pageID);
        string html = pageStorage.getPage(url);
        if(html.empty()){
            continue;
        }
        DynamicArray<PagePosting> pagePostings = buildPagePostings(html);
        for(int i = 0;i < pagePostings.getSize();i++){
            index.insert(pagePostings[i].word,pageID,pagePostings[i].frequency);
        }
        storage.savePage(pageID,pagePostings);
        lastIndexedPage = pageID;
    }
}


DynamicArray<string> Indexer::search(const string& word){
    DynamicArray<string> urls;
    if(!index.contains(word)){
        return urls;
    }
    DynamicArray<Posting>& postings = index.getPostings(word);
    for(int i = 0; i < postings.getSize(); i++){
        string url =pageStorage.getURLByID(postings[i].pageID);
        if(!url.empty()){
            urls.push_back(url);
        }
    }
    return urls;
}
