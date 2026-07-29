# InvertedIndex Design Proposal

---

# Overview

The **InvertedIndex** component is responsible for building an in-memory mapping between searchable terms and the documents in which they appear.

It receives processed terms from the **StopWordRemover** and stores their document references along with frequency information.

The InvertedIndex acts as the core search data structure of the Indexer pipeline. Its contents can later be persisted by the **IndexStorage** component and used by the Retrieval Engine for efficient search.

## Responsibilities

The InvertedIndex is responsible for:

- Maintaining the mapping between terms and documents.
- Storing posting lists for each unique term.
- Tracking term frequency within documents.
- Providing term existence checks.
- Returning postings associated with a term.
- Preparing indexed data for persistent storage.

## Non-Responsibilities

The InvertedIndex does not handle:

- HTML parsing.
- Text extraction.
- Tokenization.
- Stop-word removal.
- Persistent storage.
- Query processing or ranking.

---

# Section 1 – Public API

## Public Interface

```cpp
struct Posting
{
    int pageID;
    int frequency;
};


class InvertedIndex
{
private:

    HashMap<string, DynamicArray<Posting>> index;

public:

    void insert(
        const string& term,
        int pageID);

    bool contains(
        const string& term);

    const DynamicArray<Posting>& getPostings(
        const string& term);

    int vocabularySize();

    void clear();
};
````

### Method Description

| Method             | Parameters                       | Return Type                    | Description                                                                            |
| ------------------ | -------------------------------- | ------------------------------ | -------------------------------------------------------------------------------------- |
| `insert()`         | `const string& term, int pageID` | `void`                         | Inserts a term into the index or updates its posting frequency for the specified page. |
| `contains()`       | `const string& term`             | `bool`                         | Checks whether a term exists in the index.                                             |
| `getPostings()`    | `const string& term`             | `const DynamicArray<Posting>&` | Returns all postings associated with a term.                                           |
| `vocabularySize()` | None                             | `int`                          | Returns the total number of unique indexed terms.                                      |
| `clear()`          | None                             | `void`                         | Removes all indexed data from memory.                                                  |

---

## Why this API?

The InvertedIndex is responsible only for maintaining the mapping between searchable terms and the pages in which they appear.

It does not:

* Parse HTML.
* Tokenize text.
* Remove stop words.
* Store data permanently.

Separating these responsibilities keeps the component modular, easier to test, and easier to maintain.

---

# Section 2 – Internal Representation

The InvertedIndex maintains an in-memory index using a `HashMap`, where each unique term maps to a dynamic array of postings.

The internal structure is:

```cpp
HashMap<string, DynamicArray<Posting>>
```

The main data member is:

```cpp
HashMap<string, DynamicArray<Posting>> index;
```

The key represents a searchable term, and the value stores the list of documents containing that term.

Each posting stores:

```cpp
struct Posting
{
    int pageID;
    int frequency;
};
```

where:

* `pageID` identifies the document containing the term.
* `frequency` stores the number of occurrences of that term inside the document.

## Memory Layout

```text
                InvertedIndex

        HashMap<string, DynamicArray<Posting>>


+----------------------------------------------+
| machine                                      |
|      |                                       |
|      ▼                                       |
|   DynamicArray<Posting>                      |
|                                              |
|   +-----------------------+                  |
|   | pageID = 1 | freq = 3 |                  |
|   +-----------------------+                  |
|   | pageID = 5 | freq = 2 |                  |
|   +-----------------------+                  |
|                                              |
| learning                                     |
|      |                                       |
|      ▼                                       |
|   DynamicArray<Posting>                      |
|                                              |
|   +-----------------------+                  |
|   | pageID = 1 | freq = 2 |                  |
|   +-----------------------+                  |
|   | pageID = 4 | freq = 1 |                  |
|   +-----------------------+                  |
+----------------------------------------------+
```

---

# Index Construction

For every term received from the StopWordRemover:

1. Search the term in the HashMap.
2. If the term does not exist, create a new posting list.
3. Check whether the current page already has a posting.
4. If a posting exists, increment its frequency.
5. Otherwise, create a new `Posting` object for the page.
6. Update the posting list associated with the term.

This approach ensures that each page appears only once for a given term while accurately recording the number of occurrences.

---

# Section 3 – Failure Handling

| Situation                        | Handling Strategy                                                    |
| -------------------------------- | -------------------------------------------------------------------- |
| Empty term                       | Ignore the term.                                                     |
| Empty posting list               | Do not insert into the index.                                        |
| Duplicate term in same page      | Increment the posting frequency.                                     |
| Duplicate term in different page | Create a new posting for that page.                                  |
| Term not found during search     | Return an empty posting list or indicate absence using `contains()`. |

The InvertedIndex continues processing remaining terms even if some input is invalid.

---

# Section 4 – Complexity Analysis

Let:

* **n** = number of unique indexed terms.
* **m** = number of postings for a particular term.

| Operation                    | Best Case | Average Case | Worst Case |
| ---------------------------- | --------- | ------------ | ---------- |
| Term Lookup                  | O(1)      | O(1)         | O(n)       |
| Insert New Term              | O(1)      | O(1)         | O(n)       |
| Find Posting in Posting List | O(1)      | O(m)         | O(m)       |
| Update Frequency             | O(1)      | O(m)         | O(m)       |
| Retrieve Posting List        | O(1)      | O(1)         | O(n)       |

The HashMap provides efficient term lookup, while the posting list is searched linearly to locate an existing page entry.

---

# Section 5 – Future Compatibility

The InvertedIndex acts as the central data structure of the Indexer.

After all pages have been processed, its contents are passed to the **IndexStorage** component for persistent storage.

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
```

The Retrieval Engine will not rebuild the index. Instead, it will load the persisted index from **IndexStorage** and retrieve the posting list corresponding to a query term, enabling fast keyword-based search.

Future improvements such as:

* Term ranking.
* TF-IDF scoring.
* Phrase search support.
* Positional indexing.

can be added without changing the responsibility of existing Indexer components.
