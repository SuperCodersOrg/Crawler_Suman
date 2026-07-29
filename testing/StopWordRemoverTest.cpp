#include <gtest/gtest.h>
#include "../include/StopWordRemover.h"

#include <string>

using std::string;

//--------------------------------------------------
// Empty input
//--------------------------------------------------

TEST(StopWordRemoverTest, EmptyInput)
{
    StopWordRemover remover;

    DynamicArray<string> tokens;

    DynamicArray<string> result = remover.removeStopWords(tokens);

    EXPECT_EQ(result.getSize(), 0);
}

//--------------------------------------------------
// Remove common stop words
//--------------------------------------------------

TEST(StopWordRemoverTest, RemoveStopWords)
{
    StopWordRemover remover;

    DynamicArray<string> tokens;
    tokens.push_back("the");
    tokens.push_back("quick");
    tokens.push_back("brown");
    tokens.push_back("fox");
    tokens.push_back("is");

    DynamicArray<string> result = remover.removeStopWords(tokens);

    ASSERT_EQ(result.getSize(), 3);

    EXPECT_EQ(result.get(0), "quick");
    EXPECT_EQ(result.get(1), "brown");
    EXPECT_EQ(result.get(2), "fox");
}

//--------------------------------------------------
// Keep non-stop words
//--------------------------------------------------

TEST(StopWordRemoverTest, KeepNonStopWords)
{
    StopWordRemover remover;

    DynamicArray<string> tokens;
    tokens.push_back("computer");
    tokens.push_back("science");
    tokens.push_back("algorithm");

    DynamicArray<string> result = remover.removeStopWords(tokens);

    ASSERT_EQ(result.getSize(), 3);

    EXPECT_EQ(result.get(0), "computer");
    EXPECT_EQ(result.get(1), "science");
    EXPECT_EQ(result.get(2), "algorithm");
}

//--------------------------------------------------
// Ignore empty strings
//--------------------------------------------------

TEST(StopWordRemoverTest, IgnoreEmptyStrings)
{
    StopWordRemover remover;

    DynamicArray<string> tokens;
    tokens.push_back("");
    tokens.push_back("search");
    tokens.push_back("");
    tokens.push_back("engine");

    DynamicArray<string> result = remover.removeStopWords(tokens);

    ASSERT_EQ(result.getSize(), 2);

    EXPECT_EQ(result.get(0), "search");
    EXPECT_EQ(result.get(1), "engine");
}

//--------------------------------------------------
// Mixed input
//--------------------------------------------------

TEST(StopWordRemoverTest, MixedTokens)
{
    StopWordRemover remover;

    DynamicArray<string> tokens;
    tokens.push_back("this");
    tokens.push_back("search");
    tokens.push_back("engine");
    tokens.push_back("is");
    tokens.push_back("very");
    tokens.push_back("fast");

    DynamicArray<string> result = remover.removeStopWords(tokens);

    ASSERT_EQ(result.getSize(), 3);

    EXPECT_EQ(result.get(0), "search");
    EXPECT_EQ(result.get(1), "engine");
    EXPECT_EQ(result.get(2), "fast");
}

//--------------------------------------------------
// All stop words
//--------------------------------------------------

TEST(StopWordRemoverTest, AllStopWords)
{
    StopWordRemover remover;

    DynamicArray<string> tokens;
    tokens.push_back("the");
    tokens.push_back("is");
    tokens.push_back("and");
    tokens.push_back("of");

    DynamicArray<string> result = remover.removeStopWords(tokens);

    EXPECT_EQ(result.getSize(), 0);
}