# Daily Design Journal

**Date:** July 27, 2026

---

# 1. Specific Bug / Design Problem

Today I began designing the Indexer module, which serves as the bridge between the Crawler and the future Retrieval Engine. Before writing code, I analysed how the complete indexing workflow should be organized and identified several architectural problems that needed to be resolved.

The first challenge was determining how responsibilities should be distributed across the indexing subsystem.

Initially, I considered implementing all processing stages directly inside a single `Indexer` class.

```text
Indexer

├── Parse HTML
├── Tokenize Text
├── Remove Stop Words
├── Build Index
└── Store Index
```

Although this approach would reduce the number of classes, it would make the Indexer responsible for multiple independent tasks. As more features such as stemming, ranking, or phrase indexing were introduced, the class would become increasingly difficult to maintain.

---

### Problem 1: Lack of Separation of Responsibilities

HTML parsing, tokenization, stop-word removal, and index construction are independent operations.

Keeping them inside one class would tightly couple all processing stages.

```
Indexer

│
├── HTML Processing
├── Text Processing
├── Index Construction
├── Storage
└── Search
```

Any modification to one stage would require changes to the Indexer itself.

To improve maintainability, I decided to separate each stage into an independent component.

---

### Problem 2: Interface Between Components

Another design decision involved defining how data should flow between the preprocessing components.

Initially, I considered returning a single string after HTML parsing.

```cpp
string extractText(const string& html);
```

However, this would require the Tokenizer to perform another pass over the text to separate words.

After reviewing the workflow, I redesigned the parser to return a collection of extracted text segments.

```cpp
DynamicArray<string> extractText(const string& html);
```

This interface integrates naturally with the Tokenizer and keeps both components independent.

---

### Problem 3: HTML Processing Without External Libraries

The project required implementing an HTML parser manually instead of using an existing HTML parsing library.

This introduced several challenges:

* locating the `<body>` tag,
* skipping HTML tags,
* extracting only visible content,
* handling malformed HTML documents,
* ignoring unnecessary markup.

The parser therefore needed helper functions capable of traversing the HTML document while identifying structural elements.

---

### Problem 4: Text Normalization

The extracted text contained punctuation, mixed uppercase and lowercase letters, numbers, and empty strings.

Without normalization, different representations of the same word would be indexed separately.

For example:

```text
Computer
computer
COMPUTER
computer.
```

would become four different index entries.

The Tokenizer therefore needed to normalize text before indexing.

---

Because of these observations, I finalized a modular indexing pipeline before beginning implementation.

---

# 2. Design Decisions

After analysing the indexing workflow, I divided the system into independent components.

```
Crawler PageStorage
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
```

Each component now performs exactly one responsibility.

### HTMLParser

Responsible only for extracting visible text from HTML documents.

### Tokenizer

Responsible only for converting extracted text into normalized searchable words.

### StopWordRemover

Responsible only for removing predefined stop words.

### InvertedIndex

Responsible only for maintaining the relationship between words and pages.

### IndexStorage

Responsible only for persistence.

### Indexer

Responsible only for coordinating the workflow between all components.

This decomposition produced a much cleaner architecture than the original monolithic design.

---

# 3. Memory / Execution Diagram

```text
Stored HTML

      |
      ▼

HTMLParser

      |
      ▼

Visible Text

      |
      ▼

Tokenizer

      |
      ▼

Normalized Words

      |
      ▼

StopWordRemover

      |
      ▼

Filtered Words

```

---

# 4. Code Reference

### Commit: `c4eb2ae`

**Files:**

* `docs/designProposal/IndexerDesign/InvertedIndexDesignProposal.md`
* `docs/designProposal/IndexerDesign/IndexStorageDesignProposal.md`
* `docs/designProposal/IndexerDesign/IndexerDesignProposal.md`

Prepared the first version of the design proposals describing the architecture, responsibilities, public APIs, internal representations, complexity analysis, and future compatibility of the Indexer subsystem.

---

### Commit: `42413e8`

**Files:**

* `include/HTMLParser.h`
* `src/HTMLParser.cpp`

Implemented the HTMLParser component responsible for extracting visible textual content from HTML documents by processing the body section and ignoring HTML markup.

---

### Commit: `df26e36`

**Files:**

* `include/Tokenizer.h`
* `src/Tokenizer.cpp`

Implemented the Tokenizer component to normalize extracted text and generate searchable words for subsequent indexing stages.

---

### Commit: `0835500`

**Files:**

* `include/StopWordRemover.h`
* `src/StopWordRemover.cpp`

Implemented the StopWordRemover component using a predefined collection of stop words to eliminate low-information terms before indexing.

---

# 5. Learning Reflection

Today reinforced the importance of designing software architecture before implementation.

Initially, I focused mainly on how to implement the indexing logic. However, while analysing the complete workflow, I realized that the more important challenge was deciding where each responsibility should belong.

Separating HTML parsing, tokenization, stop-word removal, indexing, and persistence into individual components produced a much cleaner architecture than combining all operations inside the Indexer.

I also learned that carefully designing interfaces between components simplifies the overall workflow. Choosing appropriate input and output types, such as returning `DynamicArray<string>` from the HTMLParser, reduced coupling and made it easier to connect the preprocessing stages.

By completing both the architectural design and the initial preprocessing components, I established a solid foundation for implementing the remaining parts of the indexing subsystem in the following development sessions.
