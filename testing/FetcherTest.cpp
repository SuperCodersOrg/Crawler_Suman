#include <gtest/gtest.h>

#include "../include/Fetcher.h"

class FetcherTest : public ::testing::Test
{
protected:
    Fetcher fetcher;
};

TEST_F(FetcherTest, FetchValidPage)
{
    FetchResult result =
        fetcher.fetch("https://example.com");

    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.statusCode, 200);
    EXPECT_FALSE(result.html.empty());
    EXPECT_TRUE(result.errorMessage.empty());
    EXPECT_EQ(result.url, "https://example.com");
}

TEST_F(FetcherTest, FetchHTTPSPage)
{
    FetchResult result =
        fetcher.fetch("https://www.google.com");

    EXPECT_TRUE(result.success);
    EXPECT_GE(result.statusCode, 200);
    EXPECT_LT(result.statusCode, 400);
    EXPECT_FALSE(result.html.empty());
}

TEST_F(FetcherTest, InvalidDomain)
{
    FetchResult result =
        fetcher.fetch("https://this-domain-should-not-exist-123456789.com");

    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.errorMessage.empty());
}

TEST_F(FetcherTest, MalformedURL)
{
    FetchResult result =
        fetcher.fetch("not_a_url");

    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.errorMessage.empty());
}

TEST_F(FetcherTest, EmptyURL)
{
    FetchResult result =
        fetcher.fetch("");

    EXPECT_FALSE(result.success);
    EXPECT_FALSE(result.errorMessage.empty());
}

TEST_F(FetcherTest, MultipleSequentialRequests)
{
    FetchResult first =
        fetcher.fetch("https://example.com");

    FetchResult second =
        fetcher.fetch("https://example.com");

    EXPECT_TRUE(first.success);
    EXPECT_TRUE(second.success);

    EXPECT_EQ(first.statusCode, 200);
    EXPECT_EQ(second.statusCode, 200);

    EXPECT_FALSE(first.html.empty());
    EXPECT_FALSE(second.html.empty());
}