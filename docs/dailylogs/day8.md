# Daily Design Journal

**Date:** July 28, 2026

---

# 1. Specific Bug / Design Problem

After completing the preprocessing pipeline, I focused on designing the core indexing structures and the persistence layer. During implementation, I realized that simply generating searchable words was not sufficient. The system also needed an efficient way to represent relationships between words and web pages while supporting future storage and retrieval.

Initially, I considered storing each indexed word together with a list of page IDs.

```text
computer

↓

[2, 5, 8]
```

Although this representation could identify which pages contained a word, it lost an important piece of information—the number of times the word appeared in each page.

Since term frequency is an important factor for many search and ranking algorithms, this design was inadequate.

---

### Problem 1: Page Frequency Information Was Missing

The first design only recorded page identifiers.

```text
computer

↓

2
5
8
```

This meant that the following situations appeared identical:

```text
Page A

computer
```

and

```text
Page B

computer computer computer computer
```

Both pages would simply be stored as containing the word "computer", even though the second page contains the term much more frequently.

To solve this, each posting needed to store both the page identifier and the frequency of occurrence.

```text
computer

↓

(pageID = 2, frequency = 4)
```

---

### Problem 2: Mixing Temporary and Persistent Data

While designing the indexing workflow, I noticed that page processing and persistent storage required different representations.

During processing, I only needed to count word occurrences within the current page.

However, once indexing was complete, the data needed to be inserted into both the in-memory inverted index and the SQLite database.

Using a single structure for both purposes introduced unnecessary complexity.

To improve separation of responsibilities, I introduced three independent structures:

* `PagePosting`
* `Posting`
* `PostingRecord`

Each structure represented a different stage of the indexing process.

---

### Problem 3: Persistent Storage Design

Another design challenge involved deciding how the generated index should be stored.

Initially, I considered serializing the complete `InvertedIndex` into the database.

```text
InvertedIndex

↓

Store Entire Object
```

However, this tightly coupled the storage layer with the internal implementation of the index.

Any future modification to the `InvertedIndex` would require corresponding changes to the storage format.

Instead, I redesigned the persistence layer to store individual posting records.

```text
word

pageID

frequency
```

This approach simplified storage and made reconstruction of the in-memory index straightforward.

---

### Problem 4: Database Abstraction

While integrating SQLite, I wanted to avoid exposing SQL operations directly to the Indexer.

If the Indexer interacted with SQLite directly, it would become responsible for:

* opening connections,
* creating tables,
* executing SQL queries,
* handling storage errors.

This would violate the single responsibility principle.

Instead, I introduced two layers:

```text
Indexer

↓

IndexStorage

↓

Database

↓

SQLite
```

The `Database` component manages SQL operations, while `IndexStorage` provides a storage-oriented interface to the Indexer.

---

Because of these design changes, the indexing subsystem became significantly more modular and easier to extend.

---

# 2. Design Decisions

The indexing pipeline was expanded with dedicated data structures and a persistence layer.

The final workflow became:

```text
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
      ▼

InvertedIndex

      |
      ▼

IndexStorage

      |
      ▼

Database

      |
      ▼

SQLite
```

The responsibilities were clearly separated:

* `PagePosting` stores temporary word frequencies for a single page.
* `Posting` stores page-frequency pairs inside the inverted index.
* `PostingRecord` represents database records.
* `Database` manages SQLite operations.
* `IndexStorage` manages persistence.
* `Indexer` remains responsible only for coordinating the workflow.

---

# 3. Memory / Execution Diagram

```text
Current HTML Page

        |
        ▼

DynamicArray<PagePosting>

+-----------------------------+
| word        | frequency     |
+-----------------------------+
| crawler     |      2        |
| search      |      1        |
+-----------------------------+

                |
                ▼

HashMap<string, DynamicArray<Posting>>

                |
                ▼

SQLite Database

+-----------------------------+
| word | pageID | frequency   |
+-----------------------------+
```

---

# 4. Code Reference

### Commit: `fc1fc34`

**Files:**

* `include/Posting.h`
* `include/PagePosting.h`
* `include/PostingRecord.h`
* `src/Posting.cpp`
* `src/PagePosting.cpp`
* `src/PostingRecord.cpp`

Implemented the supporting data structures required for page-level processing, inverted index storage, and persistent database representation.

---

### Commit: `990353f`

**Files:**

* `include/InvertedIndex.h`
* `src/InvertedIndex.cpp`

Implemented the in-memory inverted index using `HashMap<string, DynamicArray<Posting>>` and added support for inserting words, maintaining posting lists, and retrieving indexed data.

---

### Commit: `4a1b217`

**Files:**

* `include/Database.h`
* `src/Database.cpp`

Implemented the SQLite database layer responsible for connection management, postings table creation, insertion of posting records, loading stored postings, and database maintenance operations.

---

### Commit: `4520b6b`

**Files:**

* `include/IndexStorage.h`
* `src/IndexStorage.cpp`

Implemented the `IndexStorage` component to provide a clean abstraction over the database layer, allowing the Indexer to store and restore posting information without interacting directly with SQLite.

---

### Commit: `1c7d3be`

**Files:**

* `include/Indexer.h`
* `src/Indexer.cpp`

Integrated all indexing components into a unified workflow. The `Indexer` coordinates HTML parsing, tokenization, stop-word removal, inverted index updates, and persistent storage of posting information.

---

# 5. Learning Reflection

Today's work deepened my understanding of how information retrieval systems organize and persist indexed data.

Initially, I viewed the inverted index primarily as a mapping between words and page identifiers. Through the design process, I realized that preserving term frequency is equally important because it forms the basis for many ranking techniques used by search engines.

I also learned the value of introducing intermediate data structures instead of trying to use a single representation throughout the indexing pipeline. Separating temporary page-level postings, in-memory posting lists, and persistent database records resulted in a cleaner design with well-defined responsibilities.

Finally, implementing a dedicated storage abstraction reinforced an important architectural principle: higher-level components should depend on interfaces rather than implementation details. By isolating SQLite inside the `Database` and `IndexStorage` components, the Indexer remains focused on coordinating the indexing workflow rather than managing database operations.
