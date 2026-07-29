# Build Log — Session 1

**Date:** July 28, 2026

**Duration:** 2 hours

**Goal:**
Design and implement the core data structures required for the inverted index, including posting representations and the in-memory index structure.

**Problem:**
Before implementing the complete indexing workflow, it was necessary to determine how the relationship between words and web pages would be represented in memory.

Initially, I considered directly storing page IDs against each word. However, this approach could only indicate whether a word existed in a page and could not record how many times the word appeared within that page. Since term frequency is important for future ranking algorithms, the posting structure needed to store both the page identifier and the frequency of occurrence.

Another challenge was designing a temporary representation for words extracted from a single page before inserting them into the global inverted index. This required a separate structure for page-level postings while also defining a format suitable for persistent database storage.

**What I Tried:**

* Reviewed the structure of an inverted index used in search engines.
* Analysed the information required for each indexed word.
* Designed three supporting data structures with distinct responsibilities:

  * `Posting` to represent a page ID and its corresponding term frequency.
  * `PagePosting` to maintain word-frequency pairs while processing a single page.
  * `PostingRecord` to represent posting information retrieved from persistent storage.
* Implemented constructors and data members for each structure.
* Designed the `InvertedIndex` using:

  * `HashMap<string, DynamicArray<Posting>>`
* Implemented operations to:

  * insert new words,
  * append postings for different pages,
  * update frequencies for existing postings,
  * check whether a word already exists,
  * retrieve posting lists for search operations.
* Verified that duplicate words within the same page correctly updated the frequency instead of creating duplicate posting entries.

The resulting in-memory representation became:

```text
HashMap<string, DynamicArray<Posting>>

            word
             |
             ▼

      Posting List

+----------------------+
| pageID | frequency   |
+----------------------+
|   3    |      2      |
|   8    |      1      |
+----------------------+
```

**Outcome:**

Successfully implemented the core data structures required by the indexing system.

The introduction of `Posting`, `PagePosting`, and `PostingRecord` established a clear separation between page-level processing, in-memory indexing, and persistent storage. The `InvertedIndex` was also implemented to maintain the mapping between searchable words and their associated posting lists, forming the central data structure of the indexing pipeline.

---

**Git Commits:**

**Commit ID:** `fc1fc34`

**Commit Message:**

```text
Implement posting data structures for inverted index
```

**Commit ID:** `990353f`

**Commit Message:**

```text
Implement inverted index for word-to-page mapping and search
```
