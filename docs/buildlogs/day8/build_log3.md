# Build Log — Session 3

**Date:** July 28, 2026

**Duration:** 2 hours

**Goal:**
Integrate all indexing components by implementing the `Indexer` class and establish the complete workflow from HTML page processing to persistent index storage.

**Problem:**
After implementing the individual components, the indexing system still lacked a central controller to coordinate the complete indexing process.

A major challenge was deciding how the `Indexer` should interact with `PageStorage`, `HTMLParser`, `Tokenizer`, `StopWordRemover`, `InvertedIndex`, and `IndexStorage` while keeping each component independent. The workflow also needed to ensure that every stored HTML page was processed sequentially and that both the in-memory index and persistent storage remained synchronized.

Another design consideration was ensuring that the Indexer remained responsible only for orchestration while delegating specialized tasks to the appropriate components.

**What I Tried:**

* Reviewed the interaction between all previously implemented indexing components.
* Designed the `Indexer` as the coordinator of the complete indexing pipeline.
* Implemented the workflow to:

  * retrieve stored HTML pages from `PageStorage`,
  * extract visible text using `HTMLParser`,
  * tokenize the extracted text,
  * remove stop words,
  * generate searchable terms,
  * update the `InvertedIndex`,
  * persist the generated postings using `IndexStorage`.
* Connected the output of each component to the next stage in the pipeline.
* Verified that each page was processed independently before moving to the next document.
* Tested the complete indexing workflow using previously downloaded HTML pages stored by the Crawler.

The integrated indexing pipeline became:

```text
PageStorage
     |
     ▼

HTML Page
     |
     ▼

HTMLParser
     |
     ▼

Tokenizer
     |
     ▼

StopWordRemover
     |
     ▼

InvertedIndex
     |
     ▼

IndexStorage
```

**Outcome:**

Successfully implemented the `Indexer` component and completed the first end-to-end indexing workflow.

The Indexer now coordinates all processing stages, beginning with reading stored HTML pages and ending with persistent storage of the generated index. Each component performs a single responsibility, while the Indexer manages the overall execution sequence. This completed the initial implementation of the indexing subsystem and prepared the project for further refinement, design updates, and testing.

---

**Git Commit:**

**Commit ID:** `1c7d3be`

**Commit Message:**

```text
Implement Indexer workflow with parsing, indexing, and persistence
```
