#include <gtest/gtest.h>

#include "../include/LinkExtractor.h"


class LinkExtractorTest : public ::testing::Test
{
protected:

    LinkExtractor extractor;
};


// Empty input

TEST_F(LinkExtractorTest, EmptyHTMLReturnsNoLinks)
{
    DynamicArray<string> links =
        extractor.extractLinks("");

    EXPECT_EQ(links.getSize(), 0);
}


// HTML without anchor tag

TEST_F(LinkExtractorTest, HTMLWithoutAnchorReturnsNoLinks)
{
    string html =
        "<html>"
        "<body>Hello World</body>"
        "</html>";

    DynamicArray<string> links =
        extractor.extractLinks(html);

    EXPECT_EQ(links.getSize(), 0);
}


// Basic href extraction

TEST_F(LinkExtractorTest, ExtractDoubleQuotedHref)
{
    string html =
        "<a href=\"https://google.com\">Google</a>";

    DynamicArray<string> links =
        extractor.extractLinks(html);

    ASSERT_EQ(links.getSize(), 1);

    EXPECT_EQ(
        links[0],
        "https://google.com"
    );
}


// Different href formats

TEST_F(LinkExtractorTest, ExtractDifferentHrefFormats)
{
    string html =
        "<a href='https://github.com'>GitHub</a>"
        "<a href=https://openai.com>OpenAI</a>";

    DynamicArray<string> links =
        extractor.extractLinks(html);

    ASSERT_EQ(links.getSize(), 2);

    EXPECT_EQ(
        links[0],
        "https://github.com"
    );

    EXPECT_EQ(
        links[1],
        "https://openai.com"
    );
}


// Multiple links

TEST_F(LinkExtractorTest, ExtractMultipleLinks)
{
    string html =
        "<a href=\"page1.html\">One</a>"
        "<a href=\"page2.html\">Two</a>"
        "<a href=\"page3.html\">Three</a>";

    DynamicArray<string> links =
        extractor.extractLinks(html);


    ASSERT_EQ(links.getSize(), 3);

    EXPECT_EQ(links[0], "page1.html");
    EXPECT_EQ(links[1], "page2.html");
    EXPECT_EQ(links[2], "page3.html");
}


// Case insensitive HTML

TEST_F(LinkExtractorTest, HandlesUppercaseAnchorAndHref)
{
    string html =
        "<A HREF=\"https://google.com\">Google</A>";

    DynamicArray<string> links =
        extractor.extractLinks(html);

    ASSERT_EQ(links.getSize(), 1);

    EXPECT_EQ(
        links[0],
        "https://google.com"
    );
}


// Ignore invalid tags and attributes

TEST_F(LinkExtractorTest, IgnoresNonAnchorAndInvalidHref)
{
    string html =
        "<link href=\"style.css\">"
        "<img src=\"image.png\">"
        "<a hrefx=\"wrong.html\">Wrong</a>"
        "<a href=\"correct.html\">Correct</a>";


    DynamicArray<string> links =
        extractor.extractLinks(html);


    ASSERT_EQ(links.getSize(), 1);

    EXPECT_EQ(
        links[0],
        "correct.html"
    );
}


// Whitespace handling

TEST_F(LinkExtractorTest, HandlesWhitespaceAroundEquals)
{
    string html =
        "<a    href    =    \"page.html\">";


    DynamicArray<string> links =
        extractor.extractLinks(html);


    ASSERT_EQ(links.getSize(), 1);

    EXPECT_EQ(
        links[0],
        "page.html"
    );
}


// Empty href should be ignored

TEST_F(LinkExtractorTest, IgnoresEmptyHref)
{
    string html =
        "<a href=\"\">Empty</a>";

    DynamicArray<string> links =
        extractor.extractLinks(html);

    EXPECT_EQ(
        links.getSize(),
        0
    );
}