#include <gtest/gtest.h>

#include <filesystem>

#include "../include/Crawler.h"
#include "../include/PageStorage.h"

namespace fs = std::filesystem;

class CrawlerTest : public ::testing::Test
{
protected:

    void SetUp() override
    {
        fs::remove_all("../CrawlerStorage");
        fs::remove("crawler.db");
    }

    void TearDown() override
    {
        fs::remove_all("../CrawlerStorage");
        fs::remove("crawler.db");
    }
};


// ======================================================
// Invalid URLs
// ======================================================

TEST_F(CrawlerTest, EmptyURLDoesNothing)
{
    Crawler crawler(2,10);

    EXPECT_NO_THROW(
        crawler.crawl("");
    );
}


TEST_F(CrawlerTest, WhiteSpaceURLDoesNothing)
{
    Crawler crawler(2,10);

    EXPECT_NO_THROW(
        crawler.crawl("     ");
    );
}


TEST_F(CrawlerTest, InvalidSchemeDoesNotCrash)
{
    Crawler crawler(2,10);

    EXPECT_NO_THROW(
        crawler.crawl("abcd://example");
    );
}



// ======================================================
// Reachable Websites
// ======================================================

TEST_F(CrawlerTest, CrawlGoogle)
{
    Crawler crawler(1,5);

    EXPECT_NO_THROW(
        crawler.crawl("https://google.com");
    );
}


TEST_F(CrawlerTest, CrawlExampleDotCom)
{
    Crawler crawler(1,5);

    EXPECT_NO_THROW(
        crawler.crawl("https://example.com");
    );
}


TEST_F(CrawlerTest, CrawlWikipedia)
{
    Crawler crawler(1,5);

    EXPECT_NO_THROW(
        crawler.crawl("https://wikipedia.org");
    );
}



// ======================================================
// Network Failure
// ======================================================

TEST_F(CrawlerTest, NonExistingDomainDoesNotCrash)
{
    Crawler crawler(2,10);

    EXPECT_NO_THROW(
        crawler.crawl("https://this-domain-does-not-exist-123456789.com");
    );
}



// ======================================================
// Duplicate Crawls
// ======================================================

TEST_F(CrawlerTest, CrawlingSameSeedTwiceDoesNotCrash)
{
    Crawler crawler(1,5);

    crawler.crawl("https://example.com");

    EXPECT_NO_THROW(
        crawler.crawl("https://example.com");
    );
}



// ======================================================
// URL Normalization Integration
// ======================================================

TEST_F(CrawlerTest, URLContainingSpacesDoesNotCrash)
{
    Crawler crawler(1,5);

    EXPECT_NO_THROW(
        crawler.crawl("   https://example.com   ");
    );
}



// ======================================================
// Maximum Page Limit
// ======================================================

TEST_F(CrawlerTest, MaxPageLimitOne)
{
    Crawler crawler(5,1);

    EXPECT_NO_THROW(
        crawler.crawl("https://example.com");
    );
}


TEST_F(CrawlerTest, MaxPageLimitFive)
{
    Crawler crawler(5,5);

    EXPECT_NO_THROW(
        crawler.crawl("https://example.com");
    );
}



// ======================================================
// Maximum Depth
// ======================================================

TEST_F(CrawlerTest, DepthZero)
{
    Crawler crawler(0,20);

    EXPECT_NO_THROW(
        crawler.crawl("https://example.com");
    );
}


TEST_F(CrawlerTest, DepthTwo)
{
    Crawler crawler(2,20);

    EXPECT_NO_THROW(
        crawler.crawl("https://example.com");
    );
}



// ======================================================
// HTTPS Redirect
// ======================================================

TEST_F(CrawlerTest, RedirectURL)
{
    Crawler crawler(1,5);

    EXPECT_NO_THROW(
        crawler.crawl("http://example.com");
    );
}



// ======================================================
// Persistence
// ======================================================

TEST_F(CrawlerTest, ResumeAfterRestart)
{
    {
        Crawler crawler(1,5);

        crawler.crawl("https://example.com");
    }

    EXPECT_NO_THROW(
    {
        Crawler crawler(1,5);

        crawler.crawl("https://example.com");
    });
}