#include <gtest/gtest.h>

#include "../include/Frontier.h"


// ======================================================
// URLDepth Tests
// ======================================================

TEST(URLDepthTest, StoresURLAndDepth)
{
    URLDepth item(
        "https://google.com",
        2
    );

    EXPECT_EQ(
        item.url,
        "https://google.com"
    );

    EXPECT_EQ(
        item.depth,
        2
    );
}


TEST(URLDepthTest, EqualityChecksOnlyURL)
{
    URLDepth first(
        "https://google.com",
        0
    );

    URLDepth second(
        "https://google.com",
        5
    );

    EXPECT_TRUE(
        first == second
    );
}


// ======================================================
// Frontier Tests
// ======================================================


class FrontierTest : public ::testing::Test
{
protected:

    Frontier frontier;
};



TEST_F(FrontierTest, NewFrontierIsEmpty)
{
    EXPECT_TRUE(
        frontier.empty()
    );

    EXPECT_EQ(
        frontier.size(),
        0
    );
}



TEST_F(FrontierTest, PushAndFrontReturnsCorrectURLDepth)
{
    URLDepth item(
        "https://google.com",
        1
    );


    frontier.push(item);


    EXPECT_FALSE(
        frontier.empty()
    );

    EXPECT_EQ(
        frontier.size(),
        1
    );


    EXPECT_EQ(
        frontier.front().url,
        "https://google.com"
    );


    EXPECT_EQ(
        frontier.front().depth,
        1
    );
}



TEST_F(FrontierTest, PopMaintainsFIFOOrder)
{
    URLDepth first(
        "https://google.com",
        0
    );

    URLDepth second(
        "https://github.com",
        1
    );


    frontier.push(first);
    frontier.push(second);


    URLDepth result =
        frontier.pop();


    EXPECT_EQ(
        result.url,
        "https://google.com"
    );

    EXPECT_EQ(
        result.depth,
        0
    );


    EXPECT_EQ(
        frontier.front().url,
        "https://github.com"
    );
}



TEST_F(FrontierTest, DuplicateURLIsIgnored)
{
    URLDepth first(
        "https://google.com",
        0
    );

    URLDepth duplicate(
        "https://google.com",
        5
    );


    frontier.push(first);
    frontier.push(duplicate);


    EXPECT_EQ(
        frontier.size(),
        1
    );


    EXPECT_EQ(
        frontier.pop().depth,
        0
    );
}



TEST_F(FrontierTest, ContainsTracksPendingURLs)
{
    URLDepth item(
        "https://google.com",
        0
    );


    string url =
        "https://google.com";


    EXPECT_FALSE(
        frontier.contains(url)
    );


    frontier.push(item);


    EXPECT_TRUE(
        frontier.contains(url)
    );


    frontier.pop();


    EXPECT_FALSE(
        frontier.contains(url)
    );
}



TEST_F(FrontierTest, MultipleURLsFollowFIFO)
{
    URLDepth first(
        "A",
        0
    );

    URLDepth second(
        "B",
        1
    );

    URLDepth third(
        "C",
        2
    );


    frontier.push(first);
    frontier.push(second);
    frontier.push(third);


    EXPECT_EQ(
        frontier.pop().url,
        "A"
    );

    EXPECT_EQ(
        frontier.pop().url,
        "B"
    );

    EXPECT_EQ(
        frontier.pop().url,
        "C"
    );


    EXPECT_TRUE(
        frontier.empty()
    );
}



TEST_F(FrontierTest, ReinsertAfterPopWorks)
{
    URLDepth item(
        "https://google.com",
        0
    );


    frontier.push(item);

    frontier.pop();


    frontier.push(item);


    EXPECT_EQ(
        frontier.size(),
        1
    );


    EXPECT_TRUE(
        frontier.contains(
            item.url
        )
    );
}



TEST_F(FrontierTest, SizeUpdatesCorrectly)
{
    EXPECT_EQ(
        frontier.size(),
        0
    );


    URLDepth first(
        "A",
        0
    );


    frontier.push(first);


    EXPECT_EQ(
        frontier.size(),
        1
    );


    URLDepth second(
        "B",
        1
    );


    frontier.push(second);


    EXPECT_EQ(
        frontier.size(),
        2
    );


    frontier.pop();


    EXPECT_EQ(
        frontier.size(),
        1
    );


    frontier.pop();


    EXPECT_EQ(
        frontier.size(),
        0
    );
}