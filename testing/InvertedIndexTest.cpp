#include <gtest/gtest.h>
#include "../include/InvertedIndex.h"

//--------------------------------------------------
// Empty index
//--------------------------------------------------

TEST(InvertedIndexTest, EmptyIndex)
{
    InvertedIndex index;

    EXPECT_EQ(index.vocabularySize(), 0);
    EXPECT_FALSE(index.contains("search"));
}

//--------------------------------------------------
// Insert new word
//--------------------------------------------------

TEST(InvertedIndexTest, InsertNewWord)
{
    InvertedIndex index;

    index.insert("search", 1, 3);

    EXPECT_TRUE(index.contains("search"));
    EXPECT_EQ(index.vocabularySize(), 1);

    DynamicArray<Posting>& postings = index.getPostings("search");

    ASSERT_EQ(postings.getSize(), 1);
    EXPECT_EQ(postings[0].pageID, 1);
    EXPECT_EQ(postings[0].frequency, 3);
}

//--------------------------------------------------
// Insert same word in different pages
//--------------------------------------------------

TEST(InvertedIndexTest, InsertSameWordDifferentPages)
{
    InvertedIndex index;

    index.insert("engine", 1, 2);
    index.insert("engine", 2, 5);

    EXPECT_EQ(index.vocabularySize(), 1);

    DynamicArray<Posting>& postings = index.getPostings("engine");

    ASSERT_EQ(postings.getSize(), 2);

    EXPECT_EQ(postings[0].pageID, 1);
    EXPECT_EQ(postings[0].frequency, 2);

    EXPECT_EQ(postings[1].pageID, 2);
    EXPECT_EQ(postings[1].frequency, 5);
}

//--------------------------------------------------
// Update existing posting
//--------------------------------------------------

TEST(InvertedIndexTest, UpdateExistingPosting)
{
    InvertedIndex index;

    index.insert("crawler", 1, 2);
    index.insert("crawler", 1, 7);

    DynamicArray<Posting>& postings = index.getPostings("crawler");

    ASSERT_EQ(postings.getSize(), 1);

    EXPECT_EQ(postings[0].pageID, 1);
    EXPECT_EQ(postings[0].frequency, 7);
}

//--------------------------------------------------
// Multiple words
//--------------------------------------------------

TEST(InvertedIndexTest, MultipleWords)
{
    InvertedIndex index;

    index.insert("search", 1, 2);
    index.insert("engine", 1, 3);
    index.insert("index", 2, 1);

    EXPECT_TRUE(index.contains("search"));
    EXPECT_TRUE(index.contains("engine"));
    EXPECT_TRUE(index.contains("index"));

    EXPECT_EQ(index.vocabularySize(), 3);
}

//--------------------------------------------------
// Clear index
//--------------------------------------------------

TEST(InvertedIndexTest, ClearIndex)
{
    InvertedIndex index;

    index.insert("search", 1, 2);
    index.insert("engine", 2, 1);

    index.clear();

    EXPECT_EQ(index.vocabularySize(), 0);
    EXPECT_FALSE(index.contains("search"));
    EXPECT_FALSE(index.contains("engine"));
}