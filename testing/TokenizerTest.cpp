#include <gtest/gtest.h>
#include "../include/Tokenizer.h"

#include <string>

using std::string;

//--------------------------------------------------
// Empty input
//--------------------------------------------------

TEST(TokenizerTest, EmptyInput)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    EXPECT_EQ(tokens.getSize(), 0);
}

//--------------------------------------------------
// Simple sentence
//--------------------------------------------------

TEST(TokenizerTest, TokenizeSimpleSentence)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;
    text.push_back("Hello world");

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    ASSERT_EQ(tokens.getSize(), 2);

    EXPECT_EQ(tokens.get(0), "hello");
    EXPECT_EQ(tokens.get(1), "world");
}

//--------------------------------------------------
// Remove punctuation and convert to lowercase
//--------------------------------------------------

TEST(TokenizerTest, NormalizeWords)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;
    text.push_back("Hello, WORLD! (Test)");

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    ASSERT_EQ(tokens.getSize(), 3);

    EXPECT_EQ(tokens.get(0), "hello");
    EXPECT_EQ(tokens.get(1), "world");
    EXPECT_EQ(tokens.get(2), "test");
}

//--------------------------------------------------
// Ignore numeric tokens
//--------------------------------------------------

TEST(TokenizerTest, IgnoreNumbers)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;
    text.push_back("abc 123 test 4567");

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    ASSERT_EQ(tokens.getSize(), 2);

    EXPECT_EQ(tokens.get(0), "abc");
    EXPECT_EQ(tokens.get(1), "test");
}

//--------------------------------------------------
// Ignore punctuation-only words
//--------------------------------------------------

TEST(TokenizerTest, IgnorePunctuationOnlyTokens)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;
    text.push_back("!!! ??? hello ...");

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    ASSERT_EQ(tokens.getSize(), 1);

    EXPECT_EQ(tokens.get(0), "hello");
}

//--------------------------------------------------
// Multiple input strings
//--------------------------------------------------

TEST(TokenizerTest, MultipleTextBlocks)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;
    text.push_back("First line");
    text.push_back("Second Line");

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    ASSERT_EQ(tokens.getSize(), 4);

    EXPECT_EQ(tokens.get(0), "first");
    EXPECT_EQ(tokens.get(1), "line");
    EXPECT_EQ(tokens.get(2), "second");
    EXPECT_EQ(tokens.get(3), "line");
}

//--------------------------------------------------
// Mixed words, numbers and punctuation
//--------------------------------------------------

TEST(TokenizerTest, MixedInput)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;
    text.push_back("C++ is awesome! Version 2025.");

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    ASSERT_EQ(tokens.getSize(), 4);

    EXPECT_EQ(tokens.get(0), "c");
    EXPECT_EQ(tokens.get(1), "is");
    EXPECT_EQ(tokens.get(2), "awesome");
    EXPECT_EQ(tokens.get(3), "version");
}

//--------------------------------------------------
// Hyphenated and apostrophe words
//--------------------------------------------------

TEST(TokenizerTest, HyphenAndApostropheWords)
{
    Tokenizer tokenizer;

    DynamicArray<string> text;
    text.push_back("state-of-the-art don't");

    DynamicArray<string> tokens = tokenizer.tokenize(text);

    ASSERT_EQ(tokens.getSize(), 2);

    EXPECT_EQ(tokens.get(0), "state-of-the-art");
    EXPECT_EQ(tokens.get(1), "don't");
}