# Build Log — Session 2

**Date:** July 27, 2026

**Duration:** 2 hours

**Goal:**
Implement the HTMLParser and Tokenizer components to extract visible text from HTML documents and convert it into searchable words.

**Problem:**
The first stage of the indexing pipeline requires extracting only the meaningful textual content from HTML pages. Raw HTML contains tags, scripts, styles, comments, and other markup that should not become searchable terms.

A major challenge was deciding how the parser should process HTML without relying on external parsing libraries. Since the project required implementing the functionality manually, the parser needed to correctly identify the document body, ignore unwanted elements, and return only visible text.

After extracting text, another challenge was designing the Tokenizer. The extracted text contained punctuation, mixed letter cases, numbers, and special characters. These had to be normalized so that different representations of the same word would be indexed consistently.

**What I Tried:**

* Studied the structure of HTML documents and identified which sections should be indexed.
* Designed the parser to process only the contents enclosed within the `<body>` tag.
* Implemented helper functions to:

  * locate the beginning of the body,
  * locate the end of the body,
  * detect HTML tags,
  * skip non-visible content.
* Ignored HTML markup while collecting visible characters.
* Stored extracted text using `DynamicArray<string>`.

After completing the parser:

* Designed the Tokenizer to process extracted text.
* Converted uppercase letters into lowercase.
* Split text using whitespace and punctuation as delimiters.
* Ignored empty tokens generated during splitting.
* Returned the normalized list of searchable words.

The completed preprocessing pipeline became:

```text
Raw HTML

     |
     ▼

HTMLParser

     |
     ▼

Visible Text

     |
     ▼

Tokenizer

     |
     ▼

Normalized Words
```

**Outcome:**

Successfully implemented both preprocessing components.

The `HTMLParser` extracts visible textual content from HTML documents while ignoring HTML tags and unnecessary markup.

The `Tokenizer` converts the extracted text into normalized searchable terms that can be processed by later stages of the indexing pipeline.

Together, these components established the first half of the indexing workflow and prepared the data for stop-word removal and inverted index construction.

---

**Git Commits:**

**Commit ID:** `42413e8`

**Commit Message:**

```text
Implemented HTML Parser to extract visible text from body tag
```

**Commit ID:** `df26e36`

**Commit Message:**

```text
Implemented Tokenizer to return list of words from extracted HTML text
```
