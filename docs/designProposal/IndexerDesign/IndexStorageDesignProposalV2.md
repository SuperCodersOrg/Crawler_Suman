# IndexStorage Design Proposal

---

# Overview

The **IndexStorage** component is responsible for providing permanent storage for the inverted index generated during the indexing process.

The `Indexer` generates an in-memory `InvertedIndex` while processing documents. Since this in-memory structure exists only during program execution, all indexed information would be lost after application termination. The purpose of `IndexStorage` is to store this information permanently using an SQLite database.

The current design follows an **incremental persistence model**. Instead of storing the complete inverted index after all pages are processed, the storage layer receives and stores postings generated from each page individually.

This approach provides:

- Persistent storage of indexing information.
- Recovery of previously indexed data after restart.
- Incremental indexing support.
- Reduced memory dependency for large datasets.
- Ability to continue indexing from the last processed page.

The `Indexer` acts as the coordinator between the in-memory index and persistent storage.

```text
                    Indexer

             +----------------+
             |                |
             ▼                ▼

      InvertedIndex      IndexStorage

       (Memory)            (SQLite)

````

---

# Responsibilities

The IndexStorage component is responsible for:

* Initializing the SQLite database connection.
* Creating the required database tables.
* Storing page-wise posting information.
* Loading stored posting records into an `InvertedIndex`.
* Returning the last indexed page ID.
* Clearing stored index data when required.
* Maintaining persistent word, page, and frequency relationships.

---

# Non-Responsibilities

The IndexStorage component does not perform:

* HTML parsing.
* Text extraction.
* Tokenization.
* Stop-word removal.
* Stemming.
* Index generation.
* Search query processing.
* Ranking calculations.

These operations belong to other components in the indexing pipeline.

---

# Section 1 — Public API

The proposed API is designed around incremental storage rather than complete index serialization.

A complete index storage approach would require storing the entire `InvertedIndex` object at once. However, this introduces several limitations:

* The complete index must remain available in memory.
* Partial indexing progress cannot be saved.
* Large datasets may require excessive memory.

The current design stores data as soon as a page is processed.

---

## Public Interface

```cpp
class IndexStorage
{
public:

    IndexStorage();

    bool initialize(
        const string& databaseName);


    bool savePage(
        int pageID,
        DynamicArray<PagePosting>& pagePostings);


    int load(
        InvertedIndex& index);


    void clear();
};
```

---

## Method Description

| Method           | Parameters                                              | Return Type | Description                                                       |
| ---------------- | ------------------------------------------------------- | ----------- | ----------------------------------------------------------------- |
| `IndexStorage()` | None                                                    | Constructor | Creates an IndexStorage object.                                   |
| `initialize()`   | `const string& databaseName`                            | `bool`      | Opens the SQLite database and creates required tables.            |
| `savePage()`     | `int pageID`, `DynamicArray<PagePosting>& pagePostings` | `bool`      | Stores all postings generated from a single page.                 |
| `load()`         | `InvertedIndex& index`                                  | `int`       | Restores stored postings and returns the highest indexed page ID. |
| `clear()`        | None                                                    | `void`      | Deletes all stored posting records.                               |

---

## API Design Justification

The current API separates indexing logic from storage logic.

The Indexer generates postings and sends them to storage:

```text
                Indexer

                   |
        +----------+----------+
        |                     |
        ▼                     ▼

 InvertedIndex          IndexStorage

 (Temporary)            (Permanent)

```

This design is better because:

* Each page can be stored immediately.
* The indexing process can recover after interruption.
* Storage does not depend on the internal implementation of `InvertedIndex`.
* Future database changes will not affect indexing components.

---

# Section 2 — Internal Representation

The IndexStorage uses SQLite as the persistent storage backend.

The current implementation stores individual posting records. Each record represents the occurrence of a word inside a specific page.

A posting contains:

* Word.
* Page ID.
* Frequency of occurrence.

A separate Terms table is not required because the word information is directly stored with every posting.

---

# Database Structure

```text
                     SQLite Database

                         index.db

                            |
                            ▼


                    +---------------+
                    |   Postings    |
                    +---------------+
                    | word          |
                    +---------------+
                    | pageID        |
                    +---------------+
                    | frequency     |
                    +---------------+


Example:

+-------------+--------+-----------+
| word        | pageID | frequency |
+-------------+--------+-----------+
| computer    | 10     | 3         |
| algorithm   | 10     | 2         |
| computer    | 15     | 1         |
+-------------+--------+-----------+

```

---

# Loading Representation

When `load()` is called, stored posting records are reconstructed into the in-memory inverted index.

```text
                 SQLite Database


+-------------+--------+-----------+
| word        | pageID | frequency |
+-------------+--------+-----------+
| computer    | 10     | 3         |
| algorithm   | 10     | 2         |
+-------------+--------+-----------+


                  |
                  ▼


             InvertedIndex


computer

    |
    +---- Posting
          pageID = 10
          frequency = 3



algorithm

    |
    +---- Posting
          pageID = 10
          frequency = 2

```

---

# Storage Workflow

When a page is indexed:

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

                 Indexer

              /          \

             ▼            ▼


    InvertedIndex    IndexStorage


                      |
                      ▼

                SQLite Database

```

The Indexer updates:

1. The in-memory inverted index for current execution.
2. The database for permanent storage.

---

# Section 3 — Failure Handling

IndexStorage handles failures related to database operations and stored data.

| Failure Scenario              | Handling Strategy                           |
| ----------------------------- | ------------------------------------------- |
| Database file does not exist  | SQLite creates a new database file.         |
| Database connection failure   | `initialize()` returns `false`.             |
| Table creation failure        | Initialization stops and returns `false`.   |
| Empty posting list            | No records are inserted.                    |
| Posting insertion failure     | `savePage()` returns `false`.               |
| Empty database during loading | Returns an empty index and page ID `0`.     |
| Invalid database records      | Invalid records are skipped during loading. |
| Clearing database failure     | Database reports operation failure.         |

The component ensures that storage failures do not terminate the complete indexing process unexpectedly.

---

# Section 4 — Complexity Analysis

Let:

* `p` = Number of postings generated for a single page.
* `m` = Total number of posting records stored in the database.

---

## Saving Page Data

Every posting generated from a page is inserted individually.

| Operation          | Best Case | Average Case | Worst Case |
| ------------------ | --------- | ------------ | ---------- |
| Save page postings | O(p)      | O(p)         | O(p)       |

Therefore:

```text
savePage()

O(p)
```

---

## Loading Index

During loading, every stored posting record is processed once.

| Operation                  | Best Case | Average Case | Worst Case |
| -------------------------- | --------- | ------------ | ---------- |
| Read database records      | O(m)      | O(m)         | O(m)       |
| Reconstruct inverted index | O(m)      | O(m)         | O(m)       |

Overall complexity:

```text
load()

O(m)
```

because every posting record is processed exactly once.

---

## Database Lookup

Lookup complexity depends on SQLite indexing.

Without database indexes:

| Operation            | Best Case | Average Case | Worst Case |
| -------------------- | --------- | ------------ | ---------- |
| Find posting records | O(1)      | O(m)         | O(m)       |

With database indexes:

| Operation            | Best Case | Average Case | Worst Case |
| -------------------- | --------- | ------------ | ---------- |
| Find posting records | O(1)      | O(log m)     | O(log m)   |

---

# Section 5 — Future Compatibility

The IndexStorage component is designed as an independent persistence layer between the indexing system and future search components.

The Retrieval Engine does not need to rebuild the inverted index from original documents. Instead, it can directly restore the previously generated index using the `IndexStorage::load()` method.

The Retrieval Engine only interacts with the reconstructed `InvertedIndex` and does not depend on:

* Database structure.
* Storage format.
* Index generation process.
* Document processing pipeline.

---

# Retrieval Engine Integration

The future retrieval workflow will be:

```text
              SQLite Database

                    |
                    ▼

             IndexStorage::load()

                    |
                    ▼

              InvertedIndex

                    |
                    ▼

            Retrieval Engine

                    |
                    ▼

              Search Results

```

---

# Future Extensions

The current design allows future improvements without modifying the Indexer or Retrieval Engine.

Possible extensions include:

* Adding database indexes for faster retrieval.
* Supporting batch insertion for improved indexing performance.
* Implementing incremental index updates.
* Adding index compression techniques.
* Supporting backup and recovery mechanisms.
* Migrating from SQLite to another storage backend.

Because all database operations are isolated inside `IndexStorage`, future storage changes can be implemented without affecting other components.

---

