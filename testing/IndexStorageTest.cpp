#include <gtest/gtest.h>
#include "../include/IndexStorage.h"
#include "../include/InvertedIndex.h"
#include "../include/PagePosting.h"

using std::string;

//--------------------------------------------------
// Initialize storage
//--------------------------------------------------

TEST(IndexStorageTest, InitializeDatabase)
{
    IndexStorage storage;

    EXPECT_TRUE(storage.initialize("IndexStorageTest.db"));

    storage.clear();
}

//--------------------------------------------------
// Save and load single page
//--------------------------------------------------

TEST(IndexStorageTest, SaveAndLoadSinglePage)
{
    IndexStorage storage;
    ASSERT_TRUE(storage.initialize("IndexStorageTest.db"));

    storage.clear();

    DynamicArray<PagePosting> postings;
    postings.push_back(PagePosting("search", 2));
    postings.push_back(PagePosting("engine", 3));

    EXPECT_TRUE(storage.savePage(1, postings));

    InvertedIndex index;
    int lastPage = storage.load(index);

    EXPECT_EQ(lastPage, 1);

    EXPECT_TRUE(index.contains("search"));
    EXPECT_TRUE(index.contains("engine"));

    storage.clear();
}

//--------------------------------------------------
// Save multiple pages
//--------------------------------------------------

TEST(IndexStorageTest, SaveMultiplePages)
{
    IndexStorage storage;
    ASSERT_TRUE(storage.initialize("IndexStorageTest.db"));

    storage.clear();

    DynamicArray<PagePosting> page1;
    page1.push_back(PagePosting("search", 2));

    DynamicArray<PagePosting> page2;
    page2.push_back(PagePosting("search", 5));
    page2.push_back(PagePosting("crawler", 1));

    EXPECT_TRUE(storage.savePage(1, page1));
    EXPECT_TRUE(storage.savePage(2, page2));

    InvertedIndex index;

    int lastPage = storage.load(index);

    EXPECT_EQ(lastPage, 2);
    EXPECT_EQ(index.vocabularySize(), 2);

    DynamicArray<Posting>& postings = index.getPostings("search");
    EXPECT_EQ(postings.getSize(), 2);

    storage.clear();
}

//--------------------------------------------------
// Load empty storage
//--------------------------------------------------

TEST(IndexStorageTest, LoadEmptyDatabase)
{
    IndexStorage storage;
    ASSERT_TRUE(storage.initialize("IndexStorageTest.db"));

    storage.clear();

    InvertedIndex index;

    int lastPage = storage.load(index);

    EXPECT_EQ(lastPage, 0);
    EXPECT_EQ(index.vocabularySize(), 0);
}

//--------------------------------------------------
// Clear storage
//--------------------------------------------------

TEST(IndexStorageTest, ClearStorage)
{
    IndexStorage storage;
    ASSERT_TRUE(storage.initialize("IndexStorageTest.db"));

    storage.clear();

    DynamicArray<PagePosting> postings;
    postings.push_back(PagePosting("index", 1));

    EXPECT_TRUE(storage.savePage(1, postings));

    storage.clear();

    InvertedIndex index;

    EXPECT_EQ(storage.load(index), 0);
    EXPECT_EQ(index.vocabularySize(), 0);
}

//--------------------------------------------------
// Verify frequency is preserved
//--------------------------------------------------

TEST(IndexStorageTest, PreserveFrequency)
{
    IndexStorage storage;
    ASSERT_TRUE(storage.initialize("IndexStorageTest.db"));

    storage.clear();

    DynamicArray<PagePosting> postings;
    postings.push_back(PagePosting("database", 8));

    EXPECT_TRUE(storage.savePage(5, postings));

    InvertedIndex index;

    storage.load(index);

    DynamicArray<Posting>& result = index.getPostings("database");

    ASSERT_EQ(result.getSize(), 1);
    EXPECT_EQ(result[0].pageID, 5);
    EXPECT_EQ(result[0].frequency, 8);

    storage.clear();
}