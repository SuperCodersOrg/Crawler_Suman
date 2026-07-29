# IndexStorage Design Proposal

---

# Overview

The **IndexStorage** component is responsible for permanently storing and loading the generated inverted index.

The InvertedIndex maintains data in memory during the indexing process, but this data is lost after program termination. IndexStorage provides persistence by storing the index data in an SQLite database.

This allows the Retrieval Engine (Project 04) to load the existing index directly instead of rebuilding it from raw documents.

## Responsibilities

The IndexStorage is responsible for:

- Initializing and managing the SQLite database.
- Creating required database tables.
- Saving InvertedIndex data permanently.
- Loading stored index data into memory.
- Maintaining the relationship between terms and their postings.
- Providing persistent storage between application executions.

## Non-Responsibilities

The IndexStorage does not handle:

- HTML parsing.
- Text extraction.
- Tokenization.
- Stop-word removal.
- Index construction.
- Search query processing.
- Ranking algorithms.

---

# Section 1 – Public API

## Public Interface

```cpp
class IndexStorage
{
public:

    bool initialize(
        const string& databasePath);

    bool saveIndex(
        const InvertedIndex& index);

    bool loadIndex(
        InvertedIndex& index);

    void close();
};
````

## Method Description

| Method         | Parameters                   | Return Type | Description                                                            |
| -------------- | ---------------------------- | ----------- | ---------------------------------------------------------------------- |
| `initialize()` | `const string& databasePath` | `bool`      | Opens or creates the SQLite database and prepares required tables.     |
| `saveIndex()`  | `const InvertedIndex& index` | `bool`      | Stores the complete inverted index into persistent storage.            |
| `loadIndex()`  | `InvertedIndex& index`       | `bool`      | Loads stored index data and reconstructs the in-memory inverted index. |
| `close()`      | None                         | `void`      | Closes the active database connection and releases resources.          |

---

## Why this API?

The IndexStorage exposes only storage-related operations.

It does not modify the indexing logic or understand how terms are generated. It only provides a bridge between the in-memory `InvertedIndex` and persistent database storage.

This separation keeps storage independent from indexing and allows future changes such as replacing SQLite with another storage system without affecting other Indexer components.

---

# Section 2 – Internal Representation

The IndexStorage uses **SQLite** as the persistent storage engine.

The inverted index is stored using a relational representation where terms and their associated postings are stored separately.

## Database Schema

```text
Database: index.db


Table: Terms

+----------------+
| id             |
+----------------+
| term           |
+----------------+



Table: Postings

+----------------+
| term_id        |
+----------------+
| pageID         |
+----------------+
| frequency      |
+----------------+
```

## Relationship

```text
Terms Table

+----------------+
| id = 1         |
| term = machine |
+----------------+

          |
          |
          ▼

Postings Table

+-------------------------------+
| term_id | pageID | frequency  |
+-------------------------------+
| 1       | 5      | 3          |
+-------------------------------+
| 1       | 8      | 1          |
+-------------------------------+
```

A single term can have multiple postings because it can appear in multiple documents.

---

# Storage Workflow

## Saving Index

When storing the InvertedIndex:

1. Receive the completed `InvertedIndex`.
2. Open SQLite database connection.
3. Insert each unique term into the `Terms` table.
4. Retrieve generated term IDs.
5. Insert corresponding postings into the `Postings` table.
6. Store page IDs and term frequencies.
7. Commit the database transaction.
8. Close the connection.

Example:

```text
InvertedIndex

machine

    |
    ▼

Terms Table

id = 1
term = machine


    |
    ▼

Postings Table

term_id | pageID | frequency

1       | 5      | 3
1       | 8      | 1
```

---

## Loading Index

When retrieving stored data:

1. Open SQLite database connection.
2. Read all terms from the `Terms` table.
3. Find related postings from the `Postings` table.
4. Create posting objects.
5. Insert terms and postings into a new `InvertedIndex`.
6. Return the reconstructed index.

---

# Section 3 – Failure Handling

| Situation                       | Handling Strategy                             |
| ------------------------------- | --------------------------------------------- |
| Database file does not exist    | Create a new SQLite database.                 |
| Database connection failure     | Return `false`.                               |
| Database table creation failure | Return `false`.                               |
| Empty InvertedIndex             | Store an empty database successfully.         |
| Empty database during loading   | Return an empty `InvertedIndex`.              |
| Corrupted database data         | Report failure and stop loading invalid data. |
| Missing posting information     | Ignore invalid records and continue loading.  |

The IndexStorage should prevent storage failures from crashing the complete indexing process.

---

# Section 4 – Complexity Analysis

Let:

* **n** = number of unique terms.
* **m** = total number of postings.

| Operation               | Best Case | Average Case | Worst Case |
| ----------------------- | --------- | ------------ | ---------- |
| Database Initialization | O(1)      | O(1)         | O(1)       |
| Store Terms             | O(n)      | O(n)         | O(n)       |
| Store Postings          | O(m)      | O(m)         | O(m)       |
| Load Terms              | O(n)      | O(n)         | O(n)       |
| Load Postings           | O(m)      | O(m)         | O(m)       |

The overall storage and loading complexity is:

```
O(n + m)
```

because every term and posting must be processed once.

---

# Section 5 – Future Compatibility

The IndexStorage provides persistence between the Indexer and Retrieval Engine.

```text
HTMLParser
      │
      ▼
Tokenizer
      │
      ▼
StopWordRemover
      │
      ▼
InvertedIndex
      │
      ▼
IndexStorage (SQLite)
      │
      ▼
Retrieval Engine
```

The Retrieval Engine does not rebuild the index from documents. Instead, it loads the stored index and performs searches using the existing posting lists.

Future improvements such as:

* Database indexing.
* Incremental index updates.
* Index compression.
* Backup and recovery support.
* Distributed storage.

can be implemented without changing the responsibilities of other Indexer components.

