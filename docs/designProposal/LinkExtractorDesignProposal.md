# LinkExtractor Design Proposal

# Overview

The **LinkExtractor** component is responsible for extracting hyperlinks from the HTML document downloaded by the **Fetcher**.

After a web page is successfully fetched, its HTML is passed to the LinkExtractor. The component scans the HTML document character by character, locates every valid `<a>` (anchor) tag, extracts the value of its `href` attribute, and returns all discovered URLs as a `DynamicArray<string>`.

The component intentionally performs **only extraction**. It does not determine whether a URL is valid, absolute, relative, duplicated, or crawlable. Those responsibilities belong to the **URLNormalizer**, **Frontier**, and **SeenStore** components.

Separating extraction from normalization keeps the crawler modular and makes each component responsible for a single task.

---

# Responsibilities

The LinkExtractor is responsible for:

- Parsing downloaded HTML documents.
- Detecting valid HTML anchor (`<a>`) tags.
- Identifying the `href` attribute inside an anchor tag.
- Extracting URLs stored inside the `href` attribute.
- Supporting quoted and unquoted attribute values.
- Returning every extracted URL to the crawler.

The component is **not responsible** for:

- URL normalization.
- Removing duplicate URLs.
- Checking whether URLs have already been visited.
- Downloading web pages.
- HTML validation.

---

# Design Goals

The component is designed with the following objectives:

- Keep extraction independent from normalization.
- Avoid using external HTML parsing libraries.
- Support the most common HTML hyperlink formats.
- Ignore malformed attributes whenever possible.
- Preserve the order in which hyperlinks appear.
- Keep parsing efficient with a single linear scan.

---

# Public Interface

```cpp
class LinkExtractor
{
private:

    bool isWhiteSpace(char ch);

    void skipWhiteSpace(const string& html,int& index);

    bool startsWith(
        const string& html,
        int index,
        const string& word
    );

    bool isAnchorTag(
        const string& html,
        int index
    );

public:

    DynamicArray<string> extractLinks(
        const string& html
    );
};
```

Only one public function is exposed.

```cpp
DynamicArray<string> extractLinks(const string& html);
```

Input:

- Complete HTML document.

Output:

- DynamicArray containing every extracted hyperlink.

---

# Internal Design

The implementation is divided into several helper functions to simplify parsing.

## 1. isWhiteSpace()

Determines whether a character represents whitespace.

Recognized characters include

- Space
- Tab
- Newline
- Carriage Return
- Form Feed

This helper is reused throughout the parser.

---

## 2. skipWhiteSpace()

Moves the parsing index forward until the next non-whitespace character is found.

This allows the parser to correctly process HTML such as

```html
<a      href =    "page.html">
```

---

## 3. startsWith()

Performs a case-insensitive comparison between the current HTML position and a target keyword.

Example

```
href
HREF
Href
HrEf
```

are all treated identically.

---

## 4. isAnchorTag()

Verifies whether the current parser position begins a valid opening anchor tag.

Accepted examples

```html
<a>

<a href="...">

<A HREF="...">
```

Rejected examples

```html
<abbr>

<article>

<aside>
```

The function ensures that `<a` is followed by whitespace or `>`.

---

# Link Extraction Workflow

The following diagram illustrates the complete hyperlink extraction process performed by the LinkExtractor component.

**Figure 1 & 2. Link Extractor Execution Workflow**

![Link Extractor Workflow Diagram](../images/LinkExtractor%20Workflow%20diagram.png)
![Link Extractor Workflow Diagram](../Images/LinkExtractorWorkflow.png)

The parser performs a single linear scan over the HTML document. Whenever an opening `<a>` tag is detected, it searches for the `href` attribute, extracts the URL, appends it to the output `DynamicArray<string>`, and then continues scanning until the end of the document.

---

# Component Interaction

```
             Fetcher
                │
                ▼
          Download HTML
                │
                ▼
         LinkExtractor
                │
                ▼
      Extract Raw Hyperlinks
                │
                ▼
        URLNormalizer
                │
                ▼
        Normalized URLs
                │
                ▼
           Frontier
```

The LinkExtractor never communicates directly with

- SeenStore
- Frontier
- PageStorage

It simply returns extracted URLs.

---

# Error Handling

The parser attempts to continue whenever malformed HTML is encountered.

Examples include

Missing quotation

```html
<a href="page.html>
```

Missing '='

```html
<a href "page.html">
```

Missing href

```html
<a class="button">
```

Invalid attribute

```html
<a hrefabc="page.html">
```

Such cases are skipped while the parser continues scanning the remainder of the document.

---

# Complexity Analysis

## Time Complexity

The HTML document is scanned only once.

```
O(n)
```

where

- n = number of characters in the HTML document.

---

## Space Complexity

The parser stores only the extracted URLs.

```
O(k)
```

where

- k = total size of extracted hyperlinks.

---

# Design Decisions

## Manual HTML Parsing

Instead of using an external HTML parser, the component performs lightweight manual parsing.

Advantages

- No third-party dependency.
- Small implementation.
- Faster for crawler requirements.
- Easy to integrate.

---

## Separate Extraction and Normalization

The extractor intentionally returns raw URLs.

For example

```
../about

/contact

https://Google.com

page.html?id=10
```

are returned exactly as found.

Normalization is delegated to **URLNormalizer**.

This separation follows the **Single Responsibility Principle**, making both components easier to maintain.

---

## DynamicArray as Return Type

The crawler project already uses the custom **DynamicArray** implementation from the Data Structures Library.

Returning a `DynamicArray<string>` provides:

- Consistent data structures across the project.
- Automatic resizing.
- Simple sequential iteration by the crawler.

---

# Limitations

The current implementation has several limitations.

- Only `<a>` elements are parsed.
- JavaScript-generated links are not extracted.
- HTML entities are not decoded.
- URLs inside CSS or JavaScript are ignored.
- HTML comments are not specially handled.
- Malformed HTML may cause some hyperlinks to be skipped.

These limitations are acceptable because the crawler primarily targets standard static HTML pages.

---

# Future Improvements

Possible future enhancements include

- Parsing additional hyperlink sources (`img`, `iframe`, `script`, `link`).
- Support for HTML entity decoding.
- Better malformed HTML recovery.
- Optional duplicate removal before returning URLs.
- Streaming parser for very large HTML documents.
- Multi-threaded parsing for extremely large pages.

---

# Summary

The **LinkExtractor** serves as the crawler's HTML parsing component. It efficiently scans downloaded HTML documents, identifies anchor tags, extracts hyperlink references, and returns them as a collection of raw URLs.

By limiting its responsibility to extraction alone and delegating URL normalization and duplicate detection to other components, the design remains modular, maintainable, and easy to extend as the crawler evolves.