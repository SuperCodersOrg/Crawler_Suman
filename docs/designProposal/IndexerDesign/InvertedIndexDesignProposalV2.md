# InvertedIndex Design Proposal

---

# Overview

The **InvertedIndex** component is responsible for maintaining an in-memory mapping between searchable terms and the documents in which they appear.

It receives processed terms from the **Indexer**, which coordinates the complete indexing workflow. For every processed term, the Indexer invokes the `insert()` function to update the corresponding posting list.

The InvertedIndex stores, for each unique term, the documents containing that term together with their term frequency. After all pages have been indexed, the Indexer passes the completed in-memory index to the **IndexStorage** component for persistent storage in SQLite.

The InvertedIndex serves as the core data structure of the indexing subsystem and is optimized for efficient insertion and lookup of searchable terms.

## Responsibilities

The InvertedIndex is responsible for:

* Maintaining an in-memory mapping between searchable terms and documents.
* Creating posting lists for newly encountered terms.
* Maintaining document frequency information for each indexed term.
* Updating existing postings when a page is reprocessed.
* Providing fast lookup of indexed terms.
* Returning posting lists associated with a term.
* Reporting the vocabulary size.
* Clearing the in-memory index when required.

## Non-Responsibilities

The InvertedIndex does **not** handle:

* HTML parsing.
* Text extraction.
* Tokenization.
* Stop-word removal.
* Reading HTML pages.
* Managing the indexing workflow.
* Persistent storage.
* Query processing.
* Ranking algorithms.

---

# Section 1 – Public API

## Public Interface

```cpp
struct Posting
{
    int pageID;
    int frequency;

    Posting(int pageID, int frequency);
};

class InvertedIndex
{
private:

    HashMap<string, DynamicArray<Posting>> index;

public:

    void insert(
        const string& word,
        int pageID,
        int frequency);

    bool contains(
        const string& word);

    DynamicArray<Posting>& getPostings(
        const string& word);

    int vocabularySize();

    void clear();
};
```

## Method Description

| Method             | Parameters                                      | Return Type              | Description                                                                       |
| ------------------ | ----------------------------------------------- | ------------------------ | --------------------------------------------------------------------------------- |
| `insert()`         | `const string& word, int pageID, int frequency` | `void`                   | Inserts a new posting or updates the posting corresponding to the specified page. |
| `contains()`       | `const string& word`                            | `bool`                   | Checks whether a term exists in the index.                                        |
| `getPostings()`    | `const string& word`                            | `DynamicArray<Posting>&` | Returns the posting list associated with the specified term.                      |
| `vocabularySize()` | None                                            | `int`                    | Returns the number of unique indexed terms.                                       |
| `clear()`          | None                                            | `void`                   | Removes all indexed data from memory.                                             |

## Why this API?

The InvertedIndex exposes only the operations required for maintaining an in-memory inverted index.

The component does not perform parsing, tokenization, stop-word removal, or persistent storage. These responsibilities belong to other components coordinated by the Indexer.

This separation keeps the component modular, reusable, and easy to test.

---

# Section 2 – Internal Representation

The InvertedIndex stores all indexed terms in a hash table.

```cpp
HashMap<string, DynamicArray<Posting>> index;
```

Each key represents one unique searchable term.

Each value is a posting list implemented as a `DynamicArray<Posting>`.

Each posting contains:

```cpp
struct Posting
{
    int pageID;
    int frequency;
};
```

where

* `pageID` identifies the indexed document.
* `frequency` stores the number of occurrences of the term within that document.

---

## Memory Layout

```text
HashMap<string, DynamicArray<Posting>>

machine
    │
    ▼
+-----------------------+
| pageID = 1 | freq = 5 |
+-----------------------+
| pageID = 4 | freq = 2 |
+-----------------------+

learning
    │
    ▼
+-----------------------+
| pageID = 1 | freq = 3 |
+-----------------------+
| pageID = 7 | freq = 1 |
+-----------------------+
```

Each vocabulary term appears exactly once in the HashMap.

Each posting list stores all pages containing that term.

Every page appears at most once within a posting list.

---

## Component Interaction

The Indexer controls the indexing workflow.

```text
Indexer

    │

For every HTML page

    │

HTMLParser

    │

Tokenizer

    │

StopWordRemover

    │

For every processed term

    │

InvertedIndex::insert(word, pageID, frequency)

    │

After all pages are indexed

    │

IndexStorage::saveIndex()
```

---

## Index Construction Algorithm

For every processed term received from the Indexer:

```text
Receive (word, pageID, frequency)

        │
        ▼

Does word exist?

   ┌────┴────┐
   │         │
 No         Yes
 │           │
Create     Retrieve
posting    posting list
list
             │
             ▼

Is pageID already present?

      ┌─────┴─────┐
      │           │
     Yes         No
      │           │
Update       Add new
frequency    Posting

        │
        ▼

Update HashMap
```

The algorithm ensures:

* Each vocabulary term exists only once.
* Each page appears only once within a posting list.
* Existing postings are updated when the same page is indexed again.
* New postings are created only for previously unseen pages.

---

# Section 3 – Failure Handling

| Situation                      | Handling Strategy             |
| ------------------------------ | ----------------------------- |
| Empty term                     | Ignore insertion.             |
| Invalid page ID                | Ignore insertion.             |
| New term encountered           | Create a new posting list.    |
| Existing page for a term       | Update the stored frequency.  |
| New page for an existing term  | Append a new posting.         |
| Term not present during lookup | `contains()` returns `false`. |

The InvertedIndex continues processing remaining terms even if invalid input is encountered.

---

# Section 4 – Complexity Analysis

Let

* **n** = number of unique indexed terms.
* **k** = number of postings associated with one term.

| Operation                | Average Case | Worst Case |
| ------------------------ | ------------ | ---------- |
| HashMap lookup           | O(1)         | O(n)       |
| Insert new term          | O(1)         | O(n)       |
| Search posting list      | O(k)         | O(k)       |
| Update posting frequency | O(k)         | O(k)       |
| Retrieve posting list    | O(1)         | O(n)       |
| Vocabulary size          | O(1)         | O(1)       |
| Clear index              | O(n)         | O(n)       |

Since the HashMap provides constant-time lookup under normal conditions, insertion is dominated by searching the posting list of the corresponding term.

---

# Section 5 – Future Compatibility

The InvertedIndex is managed entirely by the **Indexer**, which coordinates all indexing components.

```text
                 Indexer
                     │
     ┌───────────────┼───────────────┐
     │               │               │
HTMLParser → Tokenizer → StopWordRemover
                     │
                     ▼
              InvertedIndex
                     │
                     ▼
          IndexStorage (SQLite)
```

The Retrieval Engine does not rebuild the index. Instead, it loads the persisted index from the IndexStorage component and retrieves posting lists for user queries.

Future enhancements can be incorporated without changing the public API, including:

* TF-IDF weighting.
* BM25 ranking.
* Positional indexing.
* Phrase search.
* Wildcard search.
* Incremental indexing.
* Parallel index construction.
