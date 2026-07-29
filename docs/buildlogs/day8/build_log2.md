# Build Log — Session 2

**Date:** July 28, 2026

**Duration:** 2 hours

**Goal:**
Implement the database layer and integrate it with the `IndexStorage` component to provide persistent storage for inverted index postings.

**Problem:**
Although the `InvertedIndex` successfully maintained the mapping between words and documents in memory, the generated index was lost whenever the application terminated. A persistence mechanism was therefore required to store posting information permanently.

Initially, I considered storing the complete inverted index directly in the database. However, this approach tightly coupled the storage layer with the internal representation of the `InvertedIndex`, making future modifications difficult and preventing efficient incremental updates.

Another challenge was designing a storage component that would hide all database-specific operations from the Indexer while providing a simple interface for saving and loading posting information.

**What I Tried:**

* Reviewed how SQLite could be integrated into the indexing pipeline.
* Designed a database schema to store posting information consisting of:

  * indexed word,
  * page identifier,
  * term frequency.
* Implemented the `Database` component to:

  * establish SQLite connections,
  * create the required postings table,
  * insert posting records,
  * retrieve all stored postings,
  * clear stored posting records when required.
* Implemented the `IndexStorage` component as a wrapper around the `Database` class.
* Designed the storage API to expose only indexing-related operations while hiding SQL implementation details.
* Verified that posting records could be stored and reconstructed successfully from the database.

The storage architecture became:

```text
Indexer
     |
     ▼

IndexStorage

     |
     ▼

Database

     |
     ▼

SQLite Database
```

**Outcome:**

Successfully implemented the persistence layer for the indexing system.

The `Database` component now manages all SQLite operations, while `IndexStorage` provides a simplified interface for permanent storage and retrieval of posting information. This separation of responsibilities keeps database management independent from the indexing logic and prepares the system for future enhancements such as incremental indexing and index recovery.

---

**Git Commits:**

**Commit ID:** `4a1b217`

**Commit Message:**

```text
Implement database storage for inverted index postings
```

**Commit ID:** `4520b6b`

**Commit Message:**

```text
Implement IndexStorage for persistent inverted index management
```
