#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

#include "../../DS_Library/include/DynamicArray.h"

#include "Frontier.h"
#include "PostingRecord.h"

using namespace std;

class Database
{
private:

    sqlite3* db;

public:

    Database();
    ~Database();

    // ==============================
    // Database Connection
    // ==============================

    bool open(const string& databaseName);

    void close();

    bool execute(const string& query);

    sqlite3* getConnection();

    // ==============================
    // Crawler Database
    // ==============================

    bool createURLTable();

    bool insertPendingURL(
        const string& url,
        int depth);

    int getNextFileId();

    bool markPageStored(
        const string& url,
        int fileId,
        const string& fileName);

    bool pageExists(
        const string& url);

    string getFileName(
        const string& url);

    string getURLByID(
        int id);

    int getStoredPageCount();

    DynamicArray<URLDepth> loadPendingURLs();

    DynamicArray<string> loadCompletedURLs();

 
    bool createPostingTable();
    bool insertPosting(const string& word,int pageID,int frequency);
    DynamicArray<PostingRecord> loadAllPostings();
    void clearPostings();
};

#endif