# Build Log — Session 3

**Date:** July 27, 2026

**Duration:** 2 hours

**Goal:**
Implement the StopWordRemover component and complete the text preprocessing pipeline for the Indexer.

**Problem:**
After implementing the HTMLParser and Tokenizer, the generated tokens still contained many commonly occurring words such as *the*, *is*, *and*, and *of*. These words contribute very little to search quality but increase the size of the inverted index and the amount of storage required.

The challenge was to efficiently identify and remove these stop words while preserving the meaningful terms that would later be indexed. Another design decision was selecting an appropriate data structure for fast stop-word lookup without introducing unnecessary complexity into the preprocessing stage.

**What I Tried:**

* Studied the purpose of stop-word removal in information retrieval systems.
* Prepared a predefined list of commonly used English stop words.
* Compared different lookup approaches for checking whether a token is a stop word.
* Chose to store stop words using the custom `HashMap` from the DS Library to achieve efficient lookup during preprocessing.
* Implemented the `StopWordRemover` component to:

  * iterate through the token list,
  * check each token against the stop-word collection,
  * discard stop words,
  * preserve only meaningful searchable terms.
* Verified that the filtered output maintained the original order of non-stop words.
* Tested the complete preprocessing pipeline by passing HTML pages through:

  * HTMLParser,
  * Tokenizer,
  * StopWordRemover.

The completed preprocessing workflow became:

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
Word Tokens
     |
     ▼
StopWordRemover
     |
     ▼
Filtered Searchable Words
```

**Outcome:**

Successfully implemented the `StopWordRemover` component and completed the preprocessing stage of the indexing pipeline.

The component efficiently removes predefined stop words before indexing, reducing unnecessary entries in the inverted index and improving the quality of searchable terms.

With the completion of HTMLParser, Tokenizer, and StopWordRemover, the Indexer now has a complete preprocessing pipeline capable of transforming raw HTML documents into normalized, searchable words ready for inverted index construction.

---

**Git Commit:**

**Commit ID:** `0835500`

**Commit Message:**

```text
Implemented StopWord Remover
```
