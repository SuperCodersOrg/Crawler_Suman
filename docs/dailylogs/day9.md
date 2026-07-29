# Daily Design Journal

**Date:** July 29, 2026

---

# 1. Specific Bug / Design Problem

After completing the first implementation of the Indexer subsystem, I reviewed the architecture and compared it with the design proposals. During this review, I discovered that several design documents no longer reflected the actual implementation.

The most significant difference was in the `IndexStorage` component.

Initially, the design proposed storing and loading the complete `InvertedIndex`.

```text
InvertedIndex
      |
      ▼
IndexStorage
      |
      ▼
SQLite
```

However, the implemented solution followed a different workflow.

The Indexer processes one page at a time, generates page-specific postings, immediately stores those postings in the database, and simultaneously updates the in-memory inverted index.

```text
Page HTML
      |
      ▼
PagePosting
      |
      ├──► InvertedIndex
      |
      └──► IndexStorage
```

The original design proposal no longer described the implemented architecture.

---

### Problem 1: Design Documents Were Outdated

The first version of the design proposals had been written before the implementation was finalized.

As the implementation evolved, several design decisions changed:

* `IndexStorage` became page-oriented instead of index-oriented.
* `Indexer` introduced incremental indexing.
* Previously indexed pages could now be recovered after restarting the application.
* `InvertedIndex` maintained term frequencies instead of only page references.

Because of these changes, the documentation and implementation gradually diverged.

---

### Problem 2: Incremental Indexing Was Missing From the Design

The implemented `Indexer` maintains the variable:

```cpp
lastIndexedPage
```

During initialization:

```text
SQLite
      |
      ▼
Load Existing Postings
      |
      ▼
Determine lastIndexedPage
```

During indexing:

```text
lastIndexedPage + 1
        │
        ▼
Continue Processing
```

Only pages that have not been indexed previously are processed.

This incremental indexing strategy was completely absent from the original design proposal.

---

### Problem 3: Page-Level Posting Generation

While reviewing the implementation, I realized that an important stage of the workflow had not been documented.

Before updating the global inverted index, the Indexer first builds a temporary collection of postings for the current page.

```text
HTML

↓

Words

↓

PagePosting

↓

InvertedIndex
```

This temporary structure allows duplicate words within a page to be counted before inserting them into the global index.

The original documentation skipped this intermediate processing stage.

---

### Problem 4: Verification Through Testing

Once the implementation and documentation had been synchronized, I needed to verify that each component behaved as expected.

Testing had to cover two levels:

* Unit testing of individual components.
* Integration testing of the complete indexing workflow.

This ensured that modifications made during the redesign did not introduce regressions.

---

Because of these observations, I revised the documentation to accurately describe the implemented architecture and then developed a comprehensive testing suite.

---

# 2. Design Decisions

The final indexing workflow was updated to reflect the actual implementation.

```text
Crawler PageStorage
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

PagePosting

        |
   ┌────┴────┐
   ▼         ▼

InvertedIndex   IndexStorage

        |
        ▼

Retrieval Engine
```

The revised design now includes:

* incremental indexing,
* page-wise posting generation,
* simultaneous in-memory and persistent updates,
* recovery of previously indexed pages,
* synchronization between memory and database.

---

# 3. Memory / Execution Diagram

```text
Application Start

        |
        ▼

IndexStorage

        |
        ▼

Load Posting Records

        |
        ▼

Rebuild InvertedIndex

        |
        ▼

lastIndexedPage

        |
        ▼

Process Remaining Pages

        |
        ▼

Update Memory + Database
```

---

# 4. Code Reference

### Commit: `d00aebd`

**Files:**

* `docs/designProposal/IndexerDesign/IndexerDesignProposalV2.md`
* `docs/designProposal/IndexerDesign/InvertedIndexDesignProposalV2.md`
* `docs/designProposal/IndexerDesign/IndexStorageDesignProposalV2.md`

Updated all Indexer design proposals to reflect the implemented architecture, including revised public APIs, storage workflows, incremental indexing, failure handling, complexity analysis, and future compatibility.

---

### Commit: `f689520`

**Files:**

* `testing/HTMLParserTest.cpp`
* `testing/TokenizerTest.cpp`
* `testing/StopWordRemoverTest.cpp`
* `testing/InvertedIndexTest.cpp`
* `testing/IndexStorageTest.cpp`

Developed comprehensive unit tests covering the functionality of each Indexer component, including normal execution paths and edge cases.

---

### Commit: `0a7eeba`

**Files:**

* `testing/IndexerTest.cpp`

Implemented an integration test to validate the complete indexing workflow, ensuring that HTML pages could be processed, indexed, persisted, and searched correctly through the coordinated operation of all Indexer components.

---

# 5. Learning Reflection

Today's work emphasized that software design is an iterative process rather than a one-time activity.

Although the initial design proposals provided a good starting point, the implementation phase revealed several opportunities to simplify the architecture and improve the interaction between components. Revisiting the design after implementation helped identify mismatches between the documented architecture and the actual behaviour of the system.

I also gained a better understanding of incremental indexing. Instead of rebuilding the entire inverted index every time the application starts, maintaining the last indexed page and reconstructing the in-memory index from persistent storage significantly improves efficiency while preserving correctness.

Finally, writing comprehensive unit and integration tests reinforced the importance of verification alongside implementation. Testing not only confirmed that each component worked independently but also validated that the complete indexing pipeline—from stored HTML pages to searchable URLs—operated reliably as a unified system.
