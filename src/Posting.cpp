#include "Posting.h"

Posting::Posting(){
    pageID = 0;
    frequency = 0;
}

Posting::Posting(int pageID,int frequency){
    this->pageID = pageID;
    this->frequency = frequency;
}

bool Posting::operator==(const Posting& other) const{
    return pageID == other.pageID;
}