# Build Log — Session 1

**Date:** July 27, 2026

**Duration:** 2 hours

**Goal:**
Design the architecture of the Indexer module and prepare the initial design proposals before beginning implementation.

**Problem:**
Before implementing the Indexer, I needed to determine how the complete indexing pipeline should be organized and how responsibilities should be distributed among different components.

Initially, I considered implementing most of the processing logic inside a single `Indexer` class. However, this approach would tightly couple HTML parsing, token generation, stop-word removal, inverted index construction, and storage management into one component, making the system difficult to maintain and extend.

Another design challenge was deciding how the Indexer should interact with the Crawler developed in the previous project. The Crawler already persisted downloaded pages through `PageStorage`, so the Indexer needed a clean mechanism to consume these pages without depending on crawler internals.

It was also necessary to identify how the generated inverted index would be stored so that the future Retrieval Engine could reuse the index instead of rebuilding it every time.

**What I Tried:**

* Reviewed the requirements of Project 03 (Indexer).
* Studied the workflow of the Crawler developed in Project 02.
* Analysed how stored HTML pages would be consumed sequentially by the Indexer.
* Identified the major responsibilities involved in indexing:

  * HTML parsing
  * Tokenization
  * Stop-word removal
  * Inverted index construction
  * Persistent storage
* Compared a monolithic Indexer design with a modular architecture where each component performs a single responsibility.
* Designed the interaction between:

  * `HTMLParser`
  * `Tokenizer`
  * `StopWordRemover`
  * `InvertedIndex`
  * `IndexStorage`
  * `Indexer`
* Prepared the first version of the design proposals for:

  * InvertedIndex
  * IndexStorage
  * Indexer

The proposed indexing pipeline became:

```text
Crawler PageStorage
        |
        ▼
     Raw HTML
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
        |
        ▼
 Retrieval Engine
```

**Outcome:**

Finalized the initial architecture of the Indexer module.

The system was decomposed into independent components with clearly defined responsibilities instead of placing all functionality inside the `Indexer` class.

The finalized design established that:

* `HTMLParser` extracts visible text from HTML documents.
* `Tokenizer` converts extracted text into searchable terms.
* `StopWordRemover` filters common words.
* `InvertedIndex` maintains the mapping between terms and pages.
* `IndexStorage` provides persistent storage for the generated index.
* `Indexer` coordinates the complete indexing workflow.

This modular architecture provided a solid foundation for implementing each component independently in the subsequent sessions.

---

**Git Commit:**

**Commit ID:** `c4eb2ae`

**Commit Message:**

```text
Completed first version of Design Proposal of InvertedIndex, IndexStorage, Indexer
```
