# Fetcher Design Proposal

# Overview

The **Fetcher** component is responsible for downloading the HTML content of a web page from a given URL.

It serves as the crawler's communication layer with the web. For every URL received from the **Crawler**, the Fetcher establishes an HTTP/HTTPS connection using **libcurl**, downloads the page content, collects the HTTP response status, and returns the complete result to the crawler inside a `FetchResult` object. :contentReference[oaicite:0]{index=0} :contentReference[oaicite:1]{index=1}

The component performs only network communication. It does not parse HTML, extract hyperlinks, normalize URLs, or store downloaded pages. These responsibilities belong to the **LinkExtractor**, **URLNormalizer**, and **PageStorage** components respectively.

Separating page downloading from page processing keeps the crawler modular and follows the **Single Responsibility Principle**.

---

# Responsibilities

The Fetcher is responsible for:

- Establishing HTTP/HTTPS connections.
- Downloading HTML documents.
- Following HTTP redirects.
- Returning HTTP response status codes.
- Reporting download failures.
- Returning the downloaded HTML to the crawler.

The component is **not responsible** for:

- HTML parsing.
- URL normalization.
- Duplicate URL detection.
- Persistent storage.
- Link extraction.

---

# Design Goals

The Fetcher is designed with the following objectives:

- Provide a simple interface for downloading web pages.
- Isolate all networking logic from the crawler.
- Support both HTTP and HTTPS URLs.
- Handle redirects automatically.
- Report failures without terminating the crawler.
- Return all fetch-related information in a single result object.

---

# Public Interface

```cpp
class Fetcher
{
public:

    Fetcher();

    ~Fetcher();

    FetchResult fetch(
        const std::string& url
    );

private:

    static size_t writeCallback(
        void* contents,
        size_t size,
        size_t nmemb,
        void* userData
    );
};
```

Only one public operation is exposed.

```cpp
FetchResult fetch(const std::string& url);
```

Input

- URL to download.

Output

- A `FetchResult` containing:
  - requested URL,
  - downloaded HTML,
  - HTTP status code,
  - success flag,
  - error message (if any). :contentReference[oaicite:2]{index=2}

---

# Internal Design

The implementation consists of two major parts.

## 1. Constructor

The constructor initializes the global **libcurl** environment before any network request is performed.

If initialization fails, a runtime exception is thrown because the crawler cannot function without a networking library. :contentReference[oaicite:3]{index=3}

---

## 2. writeCallback()

During downloading, libcurl delivers the received data in multiple chunks.

The callback appends every received block into a single HTML string until the entire response has been downloaded. :contentReference[oaicite:4]{index=4}

---

## 3. fetch()

The `fetch()` function performs the complete download process.

**Figure 1. Fetcher Execution Workflow**

![Fetcher Workflow Diagram](../Images/Fetcher%20Workflow%20Diagram.png)

The workflow illustrates the complete lifecycle of a fetch request, including CURL initialization, request configuration, HTML downloading through the write callback, HTTP status retrieval, construction of the `FetchResult`, and returning the result to the crawler.

The function configures several libcurl options, including:

- Target URL
- HTML write callback
- Automatic redirect following
- Request timeout
- User-Agent
- Automatic compressed response decoding :contentReference[oaicite:5]{index=5}

---

# Component Interaction

```
Crawler
    │
    ▼
Fetcher
    │
    ▼
Download HTML
    │
    ▼
FetchResult
    │
    ▼
LinkExtractor
```

The Fetcher communicates only with the **Crawler** and returns the downloaded page through a `FetchResult`. It does not interact directly with `LinkExtractor`, `PageStorage`, `Frontier`, or `SeenStore`.

---

# Error Handling

The Fetcher reports failures through the `FetchResult` object instead of terminating the crawler.

Handled situations include:

- Invalid URL.
- Network connection failure.
- DNS resolution failure.
- Connection timeout.
- CURL initialization failure.
- HTTP server errors.

When a request fails:

- `success` remains `false`.
- `errorMessage` contains the corresponding libcurl error.
- The crawler can safely continue processing other URLs. :contentReference[oaicite:6]{index=6}

---

# Complexity Analysis

## Time Complexity

Network latency dominates execution time.

Ignoring network delay, the HTML is copied only once.

```
O(n)
```

where:

- `n` = size of the downloaded HTML.

---

## Space Complexity

The Fetcher stores the downloaded HTML before returning it.

```
O(n)
```

where:

- `n` = size of the HTML document.

---

# Design Decisions

## Using libcurl

The Fetcher uses **libcurl** as the networking library because it provides:

- HTTP and HTTPS support.
- Automatic redirect handling.
- Reliable timeout management.
- Portable cross-platform implementation.
- Mature and widely used networking APIs.

---

## FetchResult as Return Type

Instead of returning only the HTML string, the Fetcher returns a complete `FetchResult`.

Advantages include:

- Bundling HTML, URL, status code, success flag, and error message together.
- Simplifying error handling in the crawler.
- Making future extensions easier without changing the Fetcher interface.

---

## Automatic Redirect Support

HTTP redirects are followed automatically.

This allows the crawler to correctly retrieve pages whose original URLs respond with permanent or temporary redirects, reducing failed downloads.

---

# Limitations

The current implementation has several limitations.

- Downloads only static HTML.
- JavaScript-generated pages are not rendered.
- No retry mechanism for temporary network failures.
- No concurrent downloads.
- No request throttling or rate limiting.
- No HTTP caching support.

These limitations are acceptable for the current single-threaded crawler implementation.

---

# Future Improvements

Possible future enhancements include:

- Retry mechanism for transient failures.
- Concurrent page downloading.
- Configurable request headers.
- Cookie and session support.
- Proxy support.
- Dynamic content fetching using headless browsers.
- Download progress monitoring.

---

# Summary

The **Fetcher** serves as the crawler's networking component. It downloads web pages using **libcurl**, collects the HTML content and HTTP response information, and returns the result through a `FetchResult` object.

By isolating all network communication from parsing, normalization, and storage, the Fetcher provides a clean, reusable, and extensible interface that forms the first stage of the crawler's processing pipeline.