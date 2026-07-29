## Build Log - Session 3

**Date:** 24-07-2026 

**Duration:** 2 Hours

**Goal:**
Prepare detailed design proposals for the initial components of the Indexer pipeline before implementation. The objective was to define the architecture, responsibilities, APIs, processing workflow, failure handling, complexity analysis, and future extensibility of the HTMLParser, Tokenizer, and StopWordRemover components.

**Problem:**
Before beginning implementation, the Indexer required a clear software design describing how each component would interact while maintaining separation of responsibilities. It was necessary to ensure that HTML parsing, token processing, and stop-word filtering remained independent modules so that future enhancements could be made without affecting other parts of the indexing pipeline.

**What I Tried:**

* Designed the **HTMLParser** component responsible for extracting visible text from HTML documents.

* Defined the parser's public interface and documented its responsibilities and non-responsibilities.

* Designed the sequential HTML parsing workflow, including handling of `<body>`, HTML tags, scripts, styles, and comments.

* Documented parser failure handling for malformed HTML, missing body tags, and empty documents.

* Analysed the parser's linear time complexity and memory requirements.

* Designed the **Tokenizer** component responsible for converting extracted text into searchable tokens.

* Defined its public API and documented the complete tokenization pipeline.

* Specified rules for whitespace splitting, lowercase conversion, punctuation removal, numeric token filtering, and token validation.

* Documented memory flow from extracted text segments to processed tokens using `DynamicArray<string>`.

* Included failure handling scenarios and complexity analysis.

* Designed the **StopWordRemover** component responsible for filtering predefined English stop words.

* Defined its public interface and internal representation using `HashMap<string, bool>` for efficient stop-word lookup.

* Documented the filtering workflow, token lookup process, preservation of token order, and duplicate retention for later frequency calculation.

* Specified predefined stop-word categories and documented processing rules.

* Included failure handling, complexity analysis, and future extensibility considerations.

* Ensured that each component clearly documented:

  * Public API
  * Internal representation
  * Processing workflow
  * Responsibilities and non-responsibilities
  * Failure handling strategy
  * Time and space complexity
  * Future compatibility within the Indexer pipeline

* Added the completed design proposal documents to version control and committed them with the message:

  ```
  Design Proposal of HTML Parser, Tokenizer, StopWordRemover
  ```

**Outcome:**
Completed comprehensive design proposals for the first three components of the Indexer subsystem:

* HTMLParser
* Tokenizer
* StopWordRemover

The architecture now clearly defines the responsibilities and interaction of each component, establishing a modular and maintainable indexing pipeline. These design documents provide the implementation blueprint for subsequent development while ensuring that parsing, tokenization, and stop-word filtering remain loosely coupled and independently extensible.
