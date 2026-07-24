## Build Log - Session 1

**Date:** 24-07-2026  
**Duration:** 2 hours

### Goal

Develop comprehensive unit tests for the remaining crawler pipeline components—**Fetcher**, **LinkExtractor**, **URLNormalizer**, and **Frontier**—to verify their correctness independently before relying on them within the complete crawler workflow.

---

### Problem

The crawler consists of several interconnected components where the output of one module becomes the input of another. An error in any individual component could propagate through the entire crawling process. Therefore, before performing end-to-end crawler testing, each module needed to be validated independently against both normal execution paths and edge cases.

The primary objective was to ensure that:

- Fetcher correctly handles successful and failed HTTP requests.
- LinkExtractor accurately extracts hyperlinks from different HTML structures.
- URLNormalizer produces a canonical URL representation for both standalone and relative URLs while rejecting unsupported URL schemes.
- Frontier correctly manages pending URLs using FIFO order while preventing duplicate entries.

---

### What I Tried

#### 1. Fetcher Testing

Developed GoogleTest cases to verify the behaviour of the Fetcher component under multiple network scenarios.

The test suite included:

- Successful retrieval of a valid webpage.
- Fetching HTTPS websites.
- Handling of invalid or non-existent domains.
- Detection of malformed URLs.
- Empty URL validation.
- Multiple sequential fetch requests using the same Fetcher instance.

Each test verified the correctness of:

- success flag
- HTTP status code
- downloaded HTML content
- returned URL
- generated error messages

This ensured that the crawler receives consistent responses regardless of whether a request succeeds or fails.

---

#### 2. LinkExtractor Testing

Created comprehensive unit tests for the HTML parsing logic responsible for extracting hyperlinks.

The implemented tests covered:

- Empty HTML documents.
- HTML without anchor (`<a>`) tags.
- Extraction from double-quoted href attributes.
- Extraction from single-quoted href attributes.
- Extraction from unquoted href values.
- Multiple hyperlinks within a single document.
- Case-insensitive handling of HTML tags and attributes.
- Whitespace around the equals (`=`) operator.
- Ignoring invalid attributes such as `hrefx`.
- Ignoring empty href values.

These tests verified that only valid hyperlinks are extracted while malformed or unrelated HTML elements are ignored.

---

#### 3. URLNormalizer Testing

Implemented extensive test cases for URL normalization since duplicate detection throughout the crawler depends on normalized URLs.

The test suite validated:

- Conversion of scheme and hostname to lowercase.
- Removal of unnecessary trailing slashes.
- Automatic insertion of default HTTP scheme.
- Removal of surrounding whitespace.
- Removal of query parameters.
- Removal of URL fragments.
- Resolution of `.` and `..` path segments.
- Normalization of relative URLs using a base URL.
- Root-relative URL resolution.
- Protocol-relative URL resolution.
- Rejection of unsupported protocols such as:
  - `mailto:`
  - `javascript:`
  - `ftp:`
  - `tel:`
  - `data:`
- Handling of malformed or empty URLs.

These tests ensured that logically identical URLs are transformed into a single canonical representation before duplicate detection.

---

#### 4. Frontier Testing

Developed unit tests for the Frontier component to verify correct management of pending crawl URLs.

The implemented tests covered:

- Initialization of an empty frontier.
- FIFO queue behaviour.
- Correct storage of URL-depth pairs.
- Duplicate URL prevention.
- Accurate pending URL lookup using `contains()`.
- Reinsertion of URLs after removal.
- Correct queue size updates after push and pop operations.
- Sequential processing of multiple pending URLs.

The tests confirmed that duplicate URLs are prevented from entering the frontier while preserving breadth-first traversal order.

---

#### 5. Test Execution and Validation

Executed all GoogleTest suites after implementation.

Reviewed the behaviour of every component under:

- Normal execution.
- Invalid input.
- Boundary conditions.
- Duplicate data.
- Empty input.
- Error scenarios.

Verified that every component behaved according to its design specification and interacted correctly with its expected inputs and outputs.

---

### Outcome

Successfully completed comprehensive unit testing for the **Fetcher**, **LinkExtractor**, **URLNormalizer**, and **Frontier** components.

The crawler's entire URL processing pipeline—from downloading web pages, extracting hyperlinks, normalizing URLs, and managing pending crawl requests—is now independently validated through dedicated GoogleTest suites.

This significantly improves confidence in the reliability of these core modules and provides a strong foundation for subsequent integration testing and persistent crawler validation.