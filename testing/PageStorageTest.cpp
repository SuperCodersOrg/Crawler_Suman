#include <gtest/gtest.h>

#include <filesystem>

#include "../include/PageStorage.h"

namespace fs = std::filesystem;


class PageStorageTest : public ::testing::Test
{
protected:

    PageStorage* storage;


    void SetUp() override
    {
        fs::remove_all("../CrawlerStorage");

        storage = new PageStorage();
    }


    void TearDown() override
    {
        delete storage;

        fs::remove_all("../CrawlerStorage");
    }
};


// ======================================================
// Initial State Tests
// ======================================================


TEST_F(PageStorageTest, NewStorageIsEmpty)
{
    EXPECT_EQ(
        storage->pageCount(),
        0
    );


    EXPECT_FALSE(
        storage->hasPage(
            "https://google.com"
        )
    );
}


// ======================================================
// Pending URL Tests
// ======================================================


TEST_F(PageStorageTest, AddPendingURLSuccessfully)
{
    EXPECT_TRUE(
        storage->addPendingURL(
            "https://google.com",
            0
        )
    );
}



// ======================================================
// Store Page Tests
// ======================================================


TEST_F(PageStorageTest, StoreSinglePage)
{
    string url =
        "https://google.com";


    string html =
        "<html>Hello</html>";


    ASSERT_TRUE(
        storage->addPendingURL(
            url,
            0
        )
    );


    EXPECT_TRUE(
        storage->storePage(
            url,
            html,
            0
        )
    );


    EXPECT_TRUE(
        storage->hasPage(url)
    );


    EXPECT_EQ(
        storage->pageCount(),
        1
    );
}



TEST_F(PageStorageTest, RetrieveStoredHTML)
{
    string url =
        "https://example.com";


    string html =
        "<body>Hello World</body>";



    ASSERT_TRUE(
        storage->addPendingURL(
            url,
            1
        )
    );


    ASSERT_TRUE(
        storage->storePage(
            url,
            html,
            1
        )
    );


    EXPECT_EQ(
        storage->getPage(url),
        html
    );
}



TEST_F(PageStorageTest, MissingPageReturnsEmpty)
{
    EXPECT_EQ(
        storage->getPage(
            "https://missing.com"
        ),
        ""
    );
}



TEST_F(PageStorageTest, UnknownURLHasNoStoredPage)
{
    EXPECT_FALSE(
        storage->hasPage(
            "https://unknown.com"
        )
    );
}



// ======================================================
// Multiple Pages
// ======================================================


TEST_F(PageStorageTest, StoreMultiplePages)
{

    ASSERT_TRUE(
        storage->addPendingURL(
            "https://a.com",
            0
        )
    );


    ASSERT_TRUE(
        storage->addPendingURL(
            "https://b.com",
            1
        )
    );



    ASSERT_TRUE(
        storage->storePage(
            "https://a.com",
            "AAA",
            0
        )
    );


    ASSERT_TRUE(
        storage->storePage(
            "https://b.com",
            "BBB",
            1
        )
    );



    EXPECT_EQ(
        storage->pageCount(),
        2
    );


    EXPECT_EQ(
        storage->getPage("https://a.com"),
        "AAA"
    );


    EXPECT_EQ(
        storage->getPage("https://b.com"),
        "BBB"
    );
}



// ======================================================
// Edge Cases
// ======================================================


TEST_F(PageStorageTest, EmptyHTMLCanBeStored)
{
    string url =
        "https://empty.com";


    ASSERT_TRUE(
        storage->addPendingURL(
            url,
            0
        )
    );


    EXPECT_TRUE(
        storage->storePage(
            url,
            "",
            0
        )
    );


    EXPECT_TRUE(
        storage->hasPage(url)
    );


    EXPECT_EQ(
        storage->getPage(url),
        ""
    );
}



TEST_F(PageStorageTest, StorePageCreatesPersistentFile)
{
    string url =
        "https://filetest.com";


    ASSERT_TRUE(
        storage->addPendingURL(
            url,
            0
        )
    );


    ASSERT_TRUE(
        storage->storePage(
            url,
            "DATA",
            0
        )
    );


    EXPECT_TRUE(
        fs::exists(
            "../CrawlerStorage/1.page"
        )
    );
}



// ======================================================
// Recovery Tests
// ======================================================


TEST_F(PageStorageTest, RecoverCrawlerState)
{
    string pendingURL =
        "https://pending.com";


    string completedURL =
        "https://completed.com";



    ASSERT_TRUE(
        storage->addPendingURL(
            pendingURL,
            2
        )
    );



    ASSERT_TRUE(
        storage->addPendingURL(
            completedURL,
            1
        )
    );



    ASSERT_TRUE(
        storage->storePage(
            completedURL,
            "HTML",
            1
        )
    );



    Frontier frontier;

    SeenStore seen;



    storage->recoverCrawlerState(
        frontier,
        seen
    );



    EXPECT_TRUE(
        frontier.contains(
            pendingURL
        )
    );


    EXPECT_TRUE(
        seen.contains(
            completedURL
        )
    );
}



// ======================================================
// Database ID Test
// ======================================================


TEST_F(PageStorageTest, GetURLByIDReturnsCorrectURL)
{
    string url =
        "https://idtest.com";



    ASSERT_TRUE(
        storage->addPendingURL(
            url,
            0
        )
    );



    ASSERT_TRUE(
        storage->storePage(
            url,
            "HTML",
            0
        )
    );



    EXPECT_EQ(
        storage->getURLByID(1),
        url
    );
}