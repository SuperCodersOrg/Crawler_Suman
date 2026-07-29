# Indexer Design Proposal

---

# Overview

The **Indexer** component is responsible for converting raw HTML pages stored by the Crawler into a searchable inverted index.

It processes each stored page through multiple stages:

- Extracting visible text from HTML.
- Converting text into searchable terms.
- Removing unnecessary words.
- Building the term-document mapping.
- Persisting the generated index for future retrieval.

The Indexer acts as the bridge between **Crawler PageStorage** and the **Retrieval Engine**.

The Indexer follows a modular architecture where each component performs a single responsibility:

- **HTMLParser** extracts visible text content from HTML documents.
- **Tokenizer** converts extracted text into normalized searchable terms.
- **StopWordRemover** removes commonly occurring words with low search value.
- **InvertedIndex** builds the mapping between terms and documents.
- **IndexStorage** stores the generated index permanently.

The complete indexing pipeline is:

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
````

---

# Section 1 — Public API

The Indexer is divided into independent components. Each component exposes a small API according to its responsibility.

---

## HTMLParser

```cpp
class HTMLParser
{
public:

    DynamicArray<string> extractText(
        const string& html);
};
```

Extracts visible textual content from HTML documents.

---

## Tokenizer

```cpp
class Tokenizer
{
public:

    DynamicArray<string> tokenize(
        const DynamicArray<string>& text);
};
```

Converts extracted text into normalized searchable terms.

---

## StopWordRemover

```cpp
class StopWordRemover
{
public:

    DynamicArray<string> removeStopWords(
        const DynamicArray<string>& tokens);
};
```

Removes predefined stop words from tokenized data.

---

## InvertedIndex

```cpp
class InvertedIndex
{
public:

    void insert(
        const string& word,
        int pageID);

    bool contains(
        const string& word);

    const DynamicArray<Posting>& getPostings(
        const string& word);

    int vocabularySize();

    void clear();
};
```

Maintains the relationship between searchable terms and documents.

---

## IndexStorage

```cpp
class IndexStorage
{
public:

    bool saveIndex(
        const InvertedIndex& index);

    bool loadIndex(
        InvertedIndex& index);

    void close();
};
```

Provides permanent storage for the generated index.

---

# Section 2 — Internal Representation

The Indexer uses different data structures for different processing stages.

---

## HTMLParser

```text
Raw HTML

      |
      ▼

HTMLParser

      |
      ▼

DynamicArray<string>

+----------------+
| Visible Text   |
+----------------+
```

---

## Tokenizer

```text
DynamicArray<string>

+----------------+
| Text Segments  |
+----------------+

        |
        ▼

DynamicArray<string>

+------------+
| search     |
| engine     |
| crawler    |
+------------+
```

---

## StopWordRemover

```text
HashMap<string,bool>

+-------------+
| the → true  |
| is  → true  |
| and → true  |
+-------------+

        |
        ▼

Filtered Terms
```

---

## InvertedIndex

```text
HashMap<string, DynamicArray<Posting>>


term
 |
 ▼

Posting List

+----------------+
| pageID | freq  |
+----------------+
```

---

## IndexStorage

```text
SQLite Database


Terms

  |
  ▼

Postings
```

---

# Section 3 — Failure Handling

| Situation                   | Handling Strategy                              |
| --------------------------- | ---------------------------------------------- |
| Empty HTML document         | Ignore the document.                           |
| Missing body tag            | Return empty output.                           |
| Malformed HTML              | Extract readable content whenever possible.    |
| Empty token                 | Skip the token.                                |
| Stop-word only content      | Return empty output.                           |
| Duplicate term in same page | Increase frequency count.                      |
| Storage failure             | Report error without corrupting existing data. |

The Indexer continues processing remaining documents whenever possible.

---

# Section 4 — Complexity Analysis

Let:

* **n** = number of characters in HTML.
* **t** = number of generated tokens.
* **m** = number of postings for a term.

| Operation         | Complexity                   |
| ----------------- | ---------------------------- |
| HTML Parsing      | O(n)                         |
| Tokenization      | O(n)                         |
| Stop Word Removal | O(t)                         |
| Term Lookup       | O(1) average                 |
| Posting Update    | O(m)                         |
| Index Storage     | O(number of indexed entries) |

The complete indexing process operates approximately in linear time with respect to the processed document size.

---

# Section 5 — Future Compatibility

The Indexer output is consumed by the Retrieval Engine.

The complete flow is:

```text
Crawler PageStorage
        |
        ▼
      Indexer
        |
        ▼
 IndexStorage(SQLite)
        |
        ▼
 Retrieval Engine
```

The Retrieval Engine loads the persisted index instead of rebuilding it from raw HTML pages.

The architecture supports future improvements such as:

* TF-IDF ranking.
* Phrase search.
* Positional indexing.
* Incremental indexing.
* Advanced tokenization.

without changing the responsibility of existing Indexer components.

