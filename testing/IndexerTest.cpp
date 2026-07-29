#include <gtest/gtest.h>
#include "../include/Indexer.h"

using std::string;

//--------------------------------------------------
// Constructor
//--------------------------------------------------

TEST(IndexerTest, Constructor)
{
    EXPECT_NO_THROW({
        Indexer indexer;
    });
}

//--------------------------------------------------
// Search in empty index
//--------------------------------------------------

TEST(IndexerTest, SearchBeforeBuildingIndex)
{
    Indexer indexer;

    DynamicArray<string> result = indexer.search("search");

    EXPECT_EQ(result.getSize(), 0);
}

//--------------------------------------------------
// Build index
//--------------------------------------------------

TEST(IndexerTest, BuildIndex)
{
    Indexer indexer;

    EXPECT_NO_THROW({
        indexer.buildIndex();
    });
}

//--------------------------------------------------
// Search existing word
//--------------------------------------------------

TEST(IndexerTest, SearchExistingWord)
{
    Indexer indexer;

    indexer.buildIndex();

    DynamicArray<string> result = indexer.search("search");

    // If "search" exists in the indexed pages,
    // returned URLs should be valid.
    for(int i = 0; i < result.getSize(); i++){
        EXPECT_FALSE(result[i].empty());
    }
}

//--------------------------------------------------
// Search missing word
//--------------------------------------------------

TEST(IndexerTest, SearchMissingWord)
{
    Indexer indexer;

    indexer.buildIndex();

    DynamicArray<string> result =
        indexer.search("wordthatdoesnotexist");

    EXPECT_EQ(result.getSize(), 0);
}

//--------------------------------------------------
// Rebuilding should not duplicate results
//--------------------------------------------------

TEST(IndexerTest, RebuildIndex)
{
    Indexer indexer;

    indexer.buildIndex();

    DynamicArray<string> first = indexer.search("search");

    indexer.buildIndex();

    DynamicArray<string> second = indexer.search("search");

    EXPECT_EQ(first.getSize(), second.getSize());
}