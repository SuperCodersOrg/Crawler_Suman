#include <gtest/gtest.h>
#include "../include/HTMLParser.h"

#include <string>

using std::string;

//--------------------------------------------------
// Empty HTML
//--------------------------------------------------

TEST(HTMLParserTest, EmptyHTML)
{
    HTMLParser parser;

    DynamicArray<string> result = parser.extractText("");

    EXPECT_EQ(result.getSize(), 0);
}

//--------------------------------------------------
// No body tag
//--------------------------------------------------

TEST(HTMLParserTest, NoBodyTag)
{
    HTMLParser parser;

    string html =
        "<html>"
        "<head><title>Test</title></head>"
        "<p>Hello</p>"
        "</html>";

    DynamicArray<string> result = parser.extractText(html);

    EXPECT_EQ(result.getSize(), 0);
}

//--------------------------------------------------
// Extract text from simple HTML
//--------------------------------------------------

TEST(HTMLParserTest, ExtractSimpleText)
{
    HTMLParser parser;

    string html =
        "<body>"
        "<h1>Heading</h1>"
        "<p>Hello World</p>"
        "</body>";

    DynamicArray<string> result = parser.extractText(html);

    ASSERT_EQ(result.getSize(), 2);

    EXPECT_EQ(result.get(0), "Heading");
    EXPECT_EQ(result.get(1), "Hello World");
}

//--------------------------------------------------
// Nested tags
//--------------------------------------------------

TEST(HTMLParserTest, ExtractTextFromNestedTags)
{
    HTMLParser parser;

    string html =
        "<body>"
        "<div>Hello <b>World</b></div>"
        "</body>";

    DynamicArray<string> result = parser.extractText(html);

    ASSERT_EQ(result.getSize(), 2);

    EXPECT_EQ(result.get(0), "Hello");
    EXPECT_EQ(result.get(1), "World");
}

//--------------------------------------------------
// Ignore comments, script and style
//--------------------------------------------------

TEST(HTMLParserTest, IgnoreHiddenContent)
{
    HTMLParser parser;

    string html =
        "<body>"
        "Before"
        "<!-- Comment -->"
        "<script>var x = 5;</script>"
        "<style>body{color:red;}</style>"
        "After"
        "</body>";

    DynamicArray<string> result = parser.extractText(html);

    ASSERT_EQ(result.getSize(), 2);

    EXPECT_EQ(result.get(0), "Before");
    EXPECT_EQ(result.get(1), "After");
}

//--------------------------------------------------
// Body tag with attributes and uppercase
//--------------------------------------------------

TEST(HTMLParserTest, BodyAttributesAndCaseInsensitive)
{
    HTMLParser parser;

    string html =
        "<BODY class=\"main\">"
        "<P>Content</P>"
        "</BODY>";

    DynamicArray<string> result = parser.extractText(html);

    ASSERT_EQ(result.getSize(), 1);

    EXPECT_EQ(result.get(0), "Content");
}

//--------------------------------------------------
// Trim whitespace
//--------------------------------------------------

TEST(HTMLParserTest, TrimWhitespace)
{
    HTMLParser parser;

    string html =
        "<body>"
        "    Hello World    "
        "</body>";

    DynamicArray<string> result = parser.extractText(html);

    ASSERT_EQ(result.getSize(), 1);

    EXPECT_EQ(result.get(0), "Hello World");
}

//--------------------------------------------------
// Ignore whitespace-only nodes
//--------------------------------------------------

TEST(HTMLParserTest, IgnoreWhitespaceOnlyText)
{
    HTMLParser parser;

    string html =
        "<body>   \n\t   </body>";

    DynamicArray<string> result = parser.extractText(html);

    EXPECT_EQ(result.getSize(), 0);
}

//--------------------------------------------------
// Stop reading after </body>
//--------------------------------------------------

TEST(HTMLParserTest, IgnoreContentOutsideBody)
{
    HTMLParser parser;

    string html =
        "<body>Hello</body>"
        "<div>Outside</div>";

    DynamicArray<string> result = parser.extractText(html);

    ASSERT_EQ(result.getSize(), 1);

    EXPECT_EQ(result.get(0), "Hello");
}

//--------------------------------------------------
// Complex document
//--------------------------------------------------

TEST(HTMLParserTest, ComplexHTML)
{
    HTMLParser parser;

    string html =
        "<html>"
        "<head>"
        "<title>Title</title>"
        "<style>body{}</style>"
        "</head>"
        "<body>"
        "<h1>Welcome</h1>"
        "<p>Paragraph</p>"
        "<script>alert(1);</script>"
        "<div>Another Text</div>"
        "</body>"
        "</html>";

    DynamicArray<string> result = parser.extractText(html);

    ASSERT_EQ(result.getSize(), 3);

    EXPECT_EQ(result.get(0), "Welcome");
    EXPECT_EQ(result.get(1), "Paragraph");
    EXPECT_EQ(result.get(2), "Another Text");
}