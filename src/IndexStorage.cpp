#include "../include/IndexStorage.h"

IndexStorage::IndexStorage(){}

bool IndexStorage::initialize(const string& databaseName){
    if(!database.open(databaseName)){
        return false;
    }
    return database.createPostingTable();
}


bool IndexStorage::savePage(int pageID,DynamicArray<PagePosting>& pagePostings){
    for(int i = 0; i < pagePostings.getSize(); i++){
        if(!database.insertPosting(pagePostings[i].word,pageID,pagePostings[i].frequency)){
            return false;
        }
    }
    return true;
}


int IndexStorage::load(InvertedIndex& index){
    DynamicArray<PostingRecord> records = database.loadAllPostings();
    int lastIndexedPage = 0;
    for(int i = 0; i < records.getSize(); i++){
        index.insert(records[i].word,records[i].pageID,records[i].frequency);
        if(records[i].pageID > lastIndexedPage){
            lastIndexedPage =records[i].pageID;
        }
    }
    return lastIndexedPage;
}


void IndexStorage::clear(){
    database.clearPostings();
}