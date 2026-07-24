// URLNormalizerTest.cpp

#include <gtest/gtest.h>
#include "../include/URLNormalizer.h"


class URLNormalizerTest : public ::testing::Test
{
protected:
    URLNormalizer normalizer;
};


// ==================================================
// Standalone URL Normalization
// ==================================================

TEST_F(URLNormalizerTest, NormalizeStandardURLs)
{
    // Lowercase scheme and host + remove trailing slash
    EXPECT_EQ(
        normalizer.normalize("HTTPS://GOOGLE.COM/ABC/"),
        "https://google.com/ABC"
    );

    // Root URL trailing slash removal
    EXPECT_EQ(
        normalizer.normalize("https://google.com/"),
        "https://google.com"
    );

    // Add default scheme
    EXPECT_EQ(
        normalizer.normalize("google.com"),
        "http://google.com"
    );

    // Remove surrounding spaces
    EXPECT_EQ(
        normalizer.normalize("   https://google.com/test   "),
        "https://google.com/test"
    );

    // Already normalized URL
    EXPECT_EQ(
        normalizer.normalize("https://google.com/page"),
        "https://google.com/page"
    );
}


TEST_F(URLNormalizerTest, NormalizeRemovesUnnecessaryComponents)
{
    // Remove query and fragment
    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com/page?id=10#top"
        ),
        "https://google.com/page"
    );

    // Resolve dot segments
    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com/a/./b/../c"
        ),
        "https://google.com/a/c"
    );
}


TEST_F(URLNormalizerTest, RejectInvalidStandaloneURLs)
{
    EXPECT_EQ(
        normalizer.normalize(""),
        ""
    );

    EXPECT_EQ(
        normalizer.normalize("#section"),
        ""
    );

    EXPECT_EQ(
        normalizer.normalize("mailto:test@gmail.com"),
        ""
    );

    EXPECT_EQ(
        normalizer.normalize("javascript:void(0)"),
        ""
    );

    EXPECT_EQ(
        normalizer.normalize("ftp://example.com"),
        ""
    );

    EXPECT_EQ(
        normalizer.normalize("tel:12345"),
        ""
    );

    EXPECT_EQ(
        normalizer.normalize("data:image/png"),
        ""
    );
}


// ==================================================
// Base URL Normalization
// ==================================================

TEST_F(URLNormalizerTest, NormalizeRelativeURLs)
{
    // Normal relative path
    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com/a/b/index.html",
            "../page.html"
        ),
        "https://google.com/a/page.html"
    );


    // Root relative path
    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com/a/b/index.html",
            "/home"
        ),
        "https://google.com/home"
    );


    // Protocol relative URL
    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com/index.html",
            "//cdn.google.com/script.js"
        ),
        "https://cdn.google.com/script.js"
    );
}


TEST_F(URLNormalizerTest, NormalizeAbsoluteURLWithBase)
{
    EXPECT_EQ(
        normalizer.normalize(
            "https://base.com",
            "HTTPS://GOOGLE.COM/Test"
        ),
        "https://google.com/Test"
    );
}


TEST_F(URLNormalizerTest, NormalizeComplexRelativeURL)
{
    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com/a/b/index.html",
            "../c/./d.html?x=10#top"
        ),
        "https://google.com/a/c/d.html"
    );
}


TEST_F(URLNormalizerTest, RejectInvalidRelativeURLs)
{
    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com",
            ""
        ),
        ""
    );


    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com",
            "#hello"
        ),
        ""
    );


    EXPECT_EQ(
        normalizer.normalize(
            "https://google.com",
            "javascript:void(0)"
        ),
        ""
    );
}