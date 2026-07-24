#include <gtest/gtest.h>

#include "../include/SeenStore.h"


class SeenStoreTest : public ::testing::Test
{
protected:

    SeenStore seenStore;
};


// ======================================================
// Basic State Tests
// ======================================================


TEST_F(SeenStoreTest, NewSeenStoreIsEmpty)
{
    EXPECT_EQ(
        seenStore.count(),
        0
    );


    string url =
        "https://google.com";


    EXPECT_FALSE(
        seenStore.contains(url)
    );
}



// ======================================================
// Insert Tests
// ======================================================


TEST_F(SeenStoreTest, InsertURLAndCheckExistence)
{
    string url =
        "https://google.com";


    seenStore.insert(url);


    EXPECT_TRUE(
        seenStore.contains(url)
    );


    EXPECT_EQ(
        seenStore.count(),
        1
    );
}



TEST_F(SeenStoreTest, DuplicateURLDoesNotCreateMultipleEntries)
{
    string url =
        "https://google.com";


    seenStore.insert(url);
    seenStore.insert(url);


    EXPECT_TRUE(
        seenStore.contains(url)
    );


    EXPECT_EQ(
        seenStore.count(),
        1
    );
}



// ======================================================
// Multiple URL Tests
// ======================================================


TEST_F(SeenStoreTest, StoresMultipleCompletedURLs)
{
    string first =
        "https://google.com";

    string second =
        "https://github.com";

    string third =
        "https://openai.com";


    seenStore.insert(first);
    seenStore.insert(second);
    seenStore.insert(third);


    EXPECT_TRUE(
        seenStore.contains(first)
    );

    EXPECT_TRUE(
        seenStore.contains(second)
    );

    EXPECT_TRUE(
        seenStore.contains(third)
    );


    EXPECT_EQ(
        seenStore.count(),
        3
    );
}



// ======================================================
// Clear Tests
// ======================================================


TEST_F(SeenStoreTest, ClearRemovesAllURLs)
{
    string first =
        "https://google.com";

    string second =
        "https://github.com";


    seenStore.insert(first);
    seenStore.insert(second);


    seenStore.clear();


    EXPECT_EQ(
        seenStore.count(),
        0
    );


    EXPECT_FALSE(
        seenStore.contains(first)
    );


    EXPECT_FALSE(
        seenStore.contains(second)
    );
}