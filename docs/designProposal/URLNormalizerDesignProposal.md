# URLNormalizer Design Proposal

# Overview

The **URLNormalizer** component is responsible for converting raw hyperlinks extracted by the **LinkExtractor** into canonical absolute URLs that can be safely processed by the crawler.

Web pages often contain hyperlinks in different forms such as relative URLs, absolute URLs, URLs with fragments, duplicate slashes, or mixed-case hostnames. Although these URLs may refer to the same resource, treating them as different URLs would cause duplicate crawling and incorrect duplicate detection.

The URLNormalizer transforms every valid hyperlink into a consistent canonical representation before it is inserted into the **Frontier**, checked by the **SeenStore**, or stored by the **PageStorage**.

The component intentionally performs **only normalization**. It does not download pages, extract hyperlinks, detect duplicates, or store crawler state. These responsibilities belong to other crawler components.

Separating normalization from extraction improves modularity and follows the **Single Responsibility Principle**.

---

# Responsibilities

The URLNormalizer is responsible for:

- Removing leading and trailing whitespace.
- Resolving relative URLs using a base URL.
- Converting protocol and hostname to lowercase.
- Removing URL fragments (`#fragment`).
- Removing duplicate slashes from the path.
- Rejecting unsupported URL schemes.
- Rejecting malformed URLs.
- Returning a canonical absolute URL.

The component is **not responsible** for:

- Downloading web pages.
- Extracting hyperlinks.
- Detecting duplicate URLs.
- Checking whether a URL has already been visited.
- Storing URLs.

---

# Design Goals

The component is designed with the following objectives:

- Produce one canonical representation for every webpage.
- Prevent duplicate crawling caused by URL variations.
- Support both absolute and relative hyperlinks.
- Keep normalization independent from duplicate detection.
- Reject invalid or unsupported URLs early.
- Maintain linear time complexity.

---

# Public Interface

```cpp
class URLNormalizer
{
private:

    bool isWhiteSpace(char ch);

public:

    string normalize(
        string url
    );

    string normalize(
        const string& baseURL,
        const string& link
    );
};
```

Two public functions are exposed.

```cpp
string normalize(string url);
```

Input

- Absolute URL.

Output

- Canonical absolute URL.

---

```cpp
string normalize(
    const string& baseURL,
    const string& link
);
```

Input

- Base page URL.
- Hyperlink extracted from HTML.

Output

- Canonical absolute URL.

---

# Internal Design

The implementation is divided into several logical stages.

## 1. isWhiteSpace()

Determines whether a character is whitespace.

Recognized characters include

- Space
- Tab
- Newline
- Carriage Return
- Form Feed

This helper is reused while trimming URLs.

---

## 2. normalize(url)

Normalizes an already absolute URL.

Operations include

- Trim whitespace.
- Convert protocol to lowercase.
- Convert hostname to lowercase.
- Remove fragments.
- Remove duplicate slashes.
- Validate supported protocol.

---

## 3. normalize(baseURL, link)

Converts a relative hyperlink into an absolute URL before performing normalization.

Supported forms include

- Absolute URL
- Root-relative URL
- Relative URL
- Parent directory (`../`)
- Current directory (`./`)

---

# URL Normalization Algorithm

The normalization process performs one sequential scan over the URL.

**Figure 1 & 2. URL Normalizer Execution Workflow**

![URL Normalizer Workflow Diagram](../images/URL%20Normalizer%20Workflow%20Diagram.png)
![URL Normalizer Workflow Diagram](../Images/URLNormalizerWorkflow.png)

---

# Relative URL Resolution

When the extracted hyperlink is relative, the base URL is used to construct an absolute URL.

Example

Base URL

```
https://example.com/docs/page.html
```

Relative Link

```
about.html
```

Result

```
https://example.com/docs/about.html
```

---

Root-relative URL

Base URL

```
https://example.com/docs/page.html
```

Link

```
/contact
```

Result

```
https://example.com/contact
```

---

Parent directory

Base URL

```
https://example.com/docs/tutorial/page.html
```

Link

```
../images/logo.png
```

Result

```
https://example.com/docs/images/logo.png
```

---

Current directory

Base URL

```
https://example.com/docs/page.html
```

Link

```
./help.html
```

Result

```
https://example.com/docs/help.html
```

---

# Normalization Rules

## Remove Leading and Trailing Whitespace

Input

```
   https://google.com
```

Output

```
https://google.com
```

---

## Convert Protocol to Lowercase

Input

```
HTTPS://example.com
```

Output

```
https://example.com
```

---

## Convert Hostname to Lowercase

Input

```
https://Google.COM/Page
```

Output

```
https://google.com/Page
```

Only the hostname is converted to lowercase.

The URL path remains unchanged.

---

## Remove Fragment

Input

```
https://example.com/page#section
```

Output

```
https://example.com/page
```

Fragments do not identify different webpages.

---

## Remove Duplicate Slashes

Input

```
https://example.com//docs///page.html
```

Output

```
https://example.com/docs/page.html
```

The `//` following the protocol is preserved.

---

## Reject Unsupported Protocols

The following schemes are rejected.

```
javascript:
```

```
mailto:
```

```
tel:
```

```
ftp:
```

The function returns

```
""
```

---

## Reject Empty URLs

Input

```
""
```

Output

```
""
```

---

## Reject Fragment-only URLs

Input

```
#top
```

Output

```
""
```

---

# Component Interaction

```
         LinkExtractor
               │
               ▼
      Raw Hyperlinks
               │
               ▼
        URLNormalizer
               │
               ▼
      Canonical URLs
               │
      ┌────────┼────────┐
      ▼        ▼        ▼
   Frontier SeenStore PageStorage
```

The URLNormalizer never communicates directly with

- Fetcher
- Database

It simply returns normalized URLs.

---

# Error Handling

The normalizer safely rejects invalid input without terminating the crawler.

Examples include

Invalid protocol

```
javascript:void(0)
```

Malformed URL

```
https:///example.com
```

Fragment-only URL

```
#section
```

Empty URL

```
""
```

Invalid URLs are ignored by returning an empty string.

---

# Complexity Analysis

## Time Complexity

Each URL is scanned once.

```
O(n)
```

where

- n = length of the URL.

---

## Space Complexity

Only a normalized URL string is produced.

```
O(n)
```

where

- n = length of the normalized URL.

---

# Design Decisions

## Separate Normalization from Extraction

The LinkExtractor returns URLs exactly as they appear inside HTML.

Examples

```
../about
```

```
/contact
```

```
HTTPS://Google.COM
```

```
page.html?id=10
```

The URLNormalizer converts these into canonical URLs.

This separation improves maintainability and follows the **Single Responsibility Principle**.

---

## Normalize Before Duplicate Detection

Duplicate detection is performed only after normalization.

For example

```
https://google.com
```

```
HTTPS://GOOGLE.COM
```

```
https://google.com/#home
```

all normalize to

```
https://google.com
```

This prevents multiple downloads of the same webpage.

---

## Lightweight String Processing

Instead of using external URL parsing libraries, normalization is implemented using standard string operations.

Advantages include

- No third-party dependencies.
- Small implementation.
- Faster execution.
- Easy integration with the crawler.

---

# Limitations

The current implementation has several limitations.

- Does not resolve DNS aliases.
- Does not normalize query parameters.
- Does not decode URL-encoded characters.
- Does not support Internationalized Domain Names (IDN).
- Supports only standard HTTP and HTTPS URLs.

These limitations are acceptable because the crawler is designed primarily for standard static websites.

---

# Future Improvements

Possible future enhancements include

- Query parameter normalization.
- Percent-encoding normalization.
- Support for Internationalized Domain Names.
- URL canonicalization according to RFC 3986.
- Removal of default ports (80 and 443).
- Optional trailing slash normalization.

---

# Summary

The **URLNormalizer** serves as the crawler's URL canonicalization component. It converts raw hyperlinks into standardized absolute URLs by resolving relative paths, removing unnecessary variations, validating supported protocols, and rejecting malformed input.

By restricting its responsibility to URL normalization alone and delegating extraction, duplicate detection, and persistence to other components, the crawler maintains a modular architecture that is easier to maintain, test, and extend.