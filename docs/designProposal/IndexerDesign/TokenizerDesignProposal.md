# Tokenizer Design Proposal
---

# Overview

The **Tokenizer** component is responsible for converting extracted text from the HTMLParser into individual searchable terms.

It receives visible text segments extracted from HTML documents and transforms them into processed tokens that can be consumed by later Indexer components.

The Tokenizer only handles basic text processing. It does not handle HTML structure, advanced language processing, or index construction.

## Responsibilities

The Tokenizer is responsible for:

- Splitting extracted text into individual words.
- Converting tokens to lowercase.
- Removing surrounding punctuation.
- Validating tokens before storing them.
- Ignoring tokens containing only numeric values.
- Preserving meaningful alphanumeric tokens.
- Maintaining the original order of extracted terms.

## Non-Responsibilities

The Tokenizer does not handle:

- HTML parsing.
- Removal of scripts and styles.
- Stop-word removal.
- Stemming or lemmatization.
- Spelling correction.
- Inverted index construction.

## Processing Flow

```text
HTMLParser

      |
      ▼

DynamicArray<string>

      |
      ▼

Tokenizer

      |
      ▼

Processed Terms

      |
      ▼

Further Indexer Components
````

The separation of HTML parsing, token processing, and indexing allows each component to evolve independently.

---

# Section 1 – Public API

## Public Interface

```cpp
class Tokenizer
{
public:

    DynamicArray<string> tokenize(
        const DynamicArray<string>& text);
};
```

### Method Description

| Method       | Parameters                         | Return Type            | Description                                                                                                                            |
| ------------ | ---------------------------------- | ---------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| `tokenize()` | `const DynamicArray<string>& text` | `DynamicArray<string>` | Splits extracted text into individual words, converts them to lowercase, removes punctuation and validates tokens before storing them. |

### Why this API?

The Tokenizer performs only one responsibility: converting extracted text into individual searchable words.

It does not perform:

* Stop-word removal.
* Stemming.
* Indexing.

Separating these responsibilities keeps the component modular, reusable, and easy to test.

---

# Section 2 – Internal Representation

The Tokenizer processes each text segment received from the HTMLParser using `stringstream`.

Each extracted word is processed before being stored in the output `DynamicArray<string>`.

## Tokenizer Flow Diagram

![Tokenizer Flow Diagram](../../images/Tokenizer%20Flow%20Diagram.png)

## Tokenization Rules

### The Tokenizer performs the following operations:

* Reads each text segment using `stringstream`.
* Splits text based on whitespace.
* Converts every word to lowercase.
* Removes punctuation marks from the beginning and end of words.
* Ignores tokens containing only numeric values.
* Preserves meaningful alphanumeric tokens.
* Stores the processed words in a `DynamicArray<string>` while preserving their original order.

### The Tokenizer does **not**:

* Remove stop words.
* Perform stemming.
* Correct spelling.
* Build the inverted index.

These tasks are delegated to other Indexer components.

---

# Section 3 – Failure Handling

| Situation                    | Handling Strategy                        |
| ---------------------------- | ---------------------------------------- |
| Empty input array            | Return an empty `DynamicArray<string>`.  |
| Empty string                 | Ignore it and continue processing.       |
| Punctuation-only token       | Ignore the token.                        |
| Pure numeric token           | Ignore the token.                        |
| Alphanumeric token           | Preserve the token.                      |
| Multiple spaces or new lines | Automatically handled by `stringstream`. |

The Tokenizer should continue processing the remaining text even if an invalid or empty token is encountered.

---

# Section 4 – Complexity Analysis

Let:

* **n** = total number of characters in the input text.

| Operation                      | Best Case | Average Case | Worst Case |
| ------------------------------ | --------- | ------------ | ---------- |
| Read text using `stringstream` | O(n)      | O(n)         | O(n)       |
| Split into words               | O(n)      | O(n)         | O(n)       |
| Lowercase conversion           | O(n)      | O(n)         | O(n)       |
| Remove punctuation             | O(n)      | O(n)         | O(n)       |
| Overall Tokenization           | O(n)      | O(n)         | O(n)       |

Since each character is processed a constant number of times during splitting and token processing, the Tokenizer operates in **linear time**.

The output storage requirement is:

```text
O(m)
```

where **m** is the total size of generated tokens.

---

# Section 5 – Future Compatibility

The Tokenizer produces processed terms that can become the input for further Indexer components such as the **StopWordRemover**.

```text
HTMLParser
      │
      ▼
DynamicArray<string>
      │
      ▼
    Tokenizer
      │
      ▼
DynamicArray<string>
      │
      ▼
StopWordRemover
```

The Tokenizer is independent of stop-word removal and indexing.

This separation allows future enhancements such as:

* Stemming.
* Lemmatization.
* Unicode normalization.
* Language-specific tokenization.

without modifying the remaining Indexer components.

