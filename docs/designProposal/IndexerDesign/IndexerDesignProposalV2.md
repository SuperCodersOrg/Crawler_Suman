# Indexer Design Proposal

---

# Overview

The **Indexer** component is responsible for converting stored HTML pages from the Crawler's PageStorage into a searchable inverted index.

The Indexer acts as the main controller of the indexing pipeline. It coordinates multiple components responsible for different stages of processing:

- Retrieving stored HTML pages from PageStorage.
- Extracting visible text from HTML.
- Tokenizing text into searchable words.
- Removing unnecessary stop words.
- Generating page-level posting information.
- Updating the in-memory inverted index.
- Persisting generated postings into storage.

The Indexer follows an incremental indexing approach. Instead of rebuilding the complete index every time, it stores the progress of previously indexed pages and continues from the last processed page.

The complete workflow is:

```text
              Crawler PageStorage

                      |
                      ▼

                Stored HTML Page

                      |
                      ▼

                  Indexer

          +-----------+-----------+

          |           |           |

          ▼           ▼           ▼


    HTMLParser   Tokenizer   StopWordRemover


                      |
                      ▼

              Page Postings


                      |
          +-----------+-----------+

          |                       |

          ▼                       ▼


   InvertedIndex           IndexStorage


    (Memory)                (SQLite)


                      |
                      ▼

              Retrieval Engine

````

The Indexer is responsible for coordinating all indexing operations while individual components focus only on their own responsibilities.

---

# Responsibilities

The Indexer is responsible for:

* Initializing the indexing system.
* Loading previously stored index data.
* Tracking the last indexed page.
* Reading HTML pages from PageStorage.
* Processing HTML content into searchable terms.
* Generating page-level postings.
* Updating the in-memory inverted index.
* Saving generated postings permanently.
* Providing search functionality using the generated index.

---

# Non-Responsibilities

The Indexer does not handle:

* Web crawling.
* URL discovery.
* HTML downloading.
* URL normalization.
* Database management internally.
* Ranking algorithms.
* Query optimization.

These responsibilities belong to other system components.

---

# Section 1 — Public API

The Indexer exposes a simple interface because it acts as the coordinator of the complete indexing pipeline.

---

## Indexer

```cpp
class Indexer
{
public:

    Indexer();

    bool initialize(
        const string& databaseName);


    void buildIndex();


    DynamicArray<string> search(
        const string& word);
};
```

---

## Method Description

| Method         | Parameters    | Return Type            | Description                                               |
| -------------- | ------------- | ---------------------- | --------------------------------------------------------- |
| `Indexer()`    | None          | Constructor            | Initializes the Indexer and connects storage.             |
| `initialize()` | Database path | `bool`                 | Initializes storage and restores previous indexing state. |
| `buildIndex()` | None          | `void`                 | Processes new pages and updates the inverted index.       |
| `search()`     | Search word   | `DynamicArray<string>` | Returns URLs containing the given word.                   |

---

# Internal Methods

The Indexer also contains internal helper methods.

---

## buildPagePostings()

```cpp
DynamicArray<PagePosting> buildPagePostings(
    const string& html);
```

Generates posting information from a single HTML document.

Example:

Input:

```text
"computer computer science"
```

Output:

```text
[
    ("computer", 2),
    ("science", 1)
]
```

---

## findPosting()

```cpp
int findPosting(
    DynamicArray<PagePosting>& postings,
    const string& word);
```

Searches whether a word already exists in the current page posting list.

---

# Component APIs Used By Indexer

---

## HTMLParser

```cpp
DynamicArray<string> extractText(
    const string& html);
```

Extracts visible text from HTML content.

---

## Tokenizer

```cpp
DynamicArray<string> tokenize(
    const DynamicArray<string>& text);
```

Converts extracted text into normalized words.

---

## StopWordRemover

```cpp
DynamicArray<string> removeStopWords(
    const DynamicArray<string>& tokens);
```

Removes frequently occurring words with low search value.

---

## InvertedIndex

```cpp
void insert(
    const string& word,
    int pageID,
    int frequency);


bool contains(
    const string& word);


DynamicArray<Posting>& getPostings(
    const string& word);
```

Maintains the in-memory inverted index.

---

## IndexStorage

```cpp
bool initialize(
    const string& databaseName);


bool savePage(
    int pageID,
    DynamicArray<PagePosting>& pagePostings);


int load(
    InvertedIndex& index);


void clear();
```

Stores and restores posting information permanently.

---

# API Design Justification

The Indexer API is designed around the responsibility of coordinating the indexing workflow.

The Indexer does not expose internal processing functions because:

* HTML processing is handled by HTMLParser.
* Token processing is handled by Tokenizer.
* Stop-word filtering is handled by StopWordRemover.
* Data persistence is handled by IndexStorage.

This keeps each component independent and makes future changes easier.

---

# Section 2 — Internal Representation

The Indexer manages multiple data structures during execution.

---

# Page Processing Representation

When a page is processed:

```text
HTML Document


      |
      ▼


HTMLParser


      |
      ▼


DynamicArray<string>


+----------------+
| visible text   |
+----------------+


      |
      ▼


Tokenizer


      |
      ▼


DynamicArray<string>


+-------------+
| word tokens |
+-------------+

```

---

# Page Posting Representation

After stop-word removal:

```text
DynamicArray<PagePosting>


+---------------------------+
| word        | frequency   |
+---------------------------+
| computer    | 3           |
| science     | 1           |
+---------------------------+

```

This temporary structure represents the words present in a single page.

---

# InvertedIndex Representation

The final in-memory index uses:

```text
HashMap<string, DynamicArray<Posting>>


             term

              |

              ▼


        Posting List


+----------------+
| pageID| freq   |
+----------------+
| 10    | 3      |
| 15    | 1      |
+----------------+

```

Example:

```text
computer

     |
     |
     ▼

(pageID=10, frequency=3)

(pageID=15, frequency=1)

```

---

# Persistent Storage Representation

IndexStorage stores the same posting information permanently:

```text
SQLite Database


Postings Table


+-------------+--------+-----------+
| word        | pageID | frequency |
+-------------+--------+-----------+
| computer    | 10     | 3         |
| science     | 10     | 1         |
+-------------+--------+-----------+

```

---

# Indexing State Representation

The Indexer maintains:

```cpp
int lastIndexedPage;
```

This allows incremental indexing.

Example:

```text
Database contains pages:

1
2
3
4
5


lastIndexedPage = 5


Next indexing starts from:


pageID = 6

```

---

# Section 3 — Failure Handling

The Indexer handles failures at different processing stages.

| Situation                       | Handling Strategy                                       |
| ------------------------------- | ------------------------------------------------------- |
| Storage initialization failure  | Return failure from `initialize()`.                     |
| Empty HTML page                 | Skip the page and continue indexing.                    |
| Missing page from PageStorage   | Ignore the page and continue.                           |
| Malformed HTML                  | Extract available readable text.                        |
| Empty extracted text            | Generate empty posting list.                            |
| Empty token                     | Ignore invalid tokens.                                  |
| Page containing only stop words | Skip index insertion.                                   |
| Duplicate word in same page     | Increase frequency count.                               |
| Storage insertion failure       | Report failure and continue processing remaining pages. |
| Existing index data             | Load previous state before indexing new pages.          |

The Indexer attempts to continue processing whenever possible instead of stopping the complete indexing process.

---

# Section 4 — Complexity Analysis

Let:

* `n` = Number of characters in HTML page.
* `t` = Number of generated tokens.
* `p` = Number of unique words in one page.
* `m` = Total number of stored postings.

---

## HTML Processing

| Operation         | Best Case | Average Case | Worst Case |
| ----------------- | --------- | ------------ | ---------- |
| HTML Parsing      | O(n)      | O(n)         | O(n)       |
| Tokenization      | O(n)      | O(n)         | O(n)       |
| Stop Word Removal | O(t)      | O(t)         | O(t)       |

---

## Page Posting Generation

Each token is checked against the current page posting list.

| Operation               | Best Case | Average Case | Worst Case |
| ----------------------- | --------- | ------------ | ---------- |
| Find Existing Word      | O(1)      | O(p)         | O(p)       |
| Build Page Posting List | O(t)      | O(tp)        | O(tp)      |

---

## Index Update

For each unique word in a page:

| Operation      | Best Case | Average Case | Worst Case |
| -------------- | --------- | ------------ | ---------- |
| Insert Posting | O(1)      | O(m)         | O(m)       |

---

## Complete Page Indexing

For one page:

```text
O(n + t + tp + m)
```

The overall complexity depends mainly on the size of the processed document and the current index size.

---

# Section 5 — Future Compatibility

The Indexer architecture allows future search and ranking improvements without changing the indexing pipeline.

The Retrieval Engine can directly use the generated inverted index instead of processing documents again.

The future workflow is:

```text
              IndexStorage

                    |
                    ▼

             Load InvertedIndex

                    |
                    ▼

            Retrieval Engine

                    |
                    ▼

             Search Processing

```

---

# Future Improvements

The current design supports future extensions such as:

* TF-IDF based ranking.
* Phrase searching.
* Positional indexes.
* Query optimization.
* Incremental index updates.
* Parallel page processing.
* Index compression.

These improvements can be added without changing the responsibilities of existing components because:

* HTMLParser handles document extraction.
* Tokenizer handles word generation.
* StopWordRemover handles filtering.
* InvertedIndex handles term relationships.
* IndexStorage handles persistence.
* Indexer coordinates the workflow.

---
