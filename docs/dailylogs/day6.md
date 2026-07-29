# Daily Design Journal

**Date:** 24-07-2026

---

## Section 1 — Specific Bug

No compiler errors or runtime crashes were encountered during today's work. Instead, the following engineering issues were identified during development and project review:

* The crawler workflow documentation did not accurately represent the implemented persistence and crawler recovery mechanism.
* Design documentation for the **Fetcher**, **LinkExtractor**, and **URLNormalizer** components was missing despite their implementation.
* Before implementing the Indexer subsystem, the interaction between **HTMLParser**, **Tokenizer**, and **StopWordRemover** was not formally defined, creating a risk of overlapping responsibilities.
* Comprehensive unit tests were still required to verify the correctness of the crawler pipeline components independently before integration.

---

## Section 2 — Failed Attempt

Initially, I considered validating the crawler mainly through end-to-end execution. However, this approach was not suitable because failures in one component could propagate through the pipeline, making it difficult to determine the actual source of an error.

During documentation review, I also noticed that the documented crawler workflow assumed behaviour that differed from the implemented persistence logic. Similarly, beginning Indexer implementation without a formal component design would have tightly coupled parsing, tokenization, and stop-word filtering.

To resolve these issues, I first completed independent GoogleTest suites for every crawler component, then updated the crawler design documentation to match the implementation, and finally prepared detailed design proposals for the first Indexer components before writing their implementation.

---

## Section 3 — Memory Diagram

**Weekly hand-drawn memory diagram attached separately.**

Diagram represented:

```text
                 Raw HTML
                     │
                     ▼
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
                     │
                     ▼
        DynamicArray<string>
                     │
                     ▼
             InvertedIndex
```

The diagram illustrates how data flows through the indexing pipeline while each component maintains a single responsibility.

---

## Section 4 — Code Reference

**Commit Hash**

* `f590e0c` — Design Proposal of HTML Parser, Tokenizer, StopWordRemover

**Files Modified**

* `docs/designProposal/IndexerDesign/HTMLParserDesignProposal.md`
* `docs/designProposal/IndexerDesign/TokenizerDesignProposal.md`
* `docs/designProposal/IndexerDesign/StopWordRemoverDesignProposal.md`

**Additional Files Worked On**

* `tests/FetcherTest.cpp`
* `tests/LinkExtractorTest.cpp`
* `tests/URLNormalizerTest.cpp`
* `tests/FrontierTest.cpp`
* `tests/PageStorageTest.cpp`
* `tests/SeenStoreTest.cpp`
* `docs/designProposal/CrawlerDesign/FetcherDesignProposal.md`
* `docs/designProposal/CrawlerDesign/LinkExtractorDesignProposal.md`
* `docs/designProposal/CrawlerDesign/URLNormalizerDesignProposal.md`

**Relevant Code Sections**

* GoogleTest test cases validating Fetcher, LinkExtractor, URLNormalizer, Frontier, PageStorage, and SeenStore.
* Updated crawler workflow documentation describing persistent recovery using `initializeCrawler()` and `recoverCrawlerState()`.
* Complete design proposal sections defining public APIs, processing workflows, internal representations, failure handling, complexity analysis, and future compatibility for HTMLParser, Tokenizer, and StopWordRemover.

---

## Section 5 — Learning Reflection

Today's work strengthened my understanding that software architecture, testing, and documentation should evolve together rather than independently. Writing unit tests before integration demonstrated how isolating each crawler component makes it much easier to verify correctness and identify problems without interference from other modules. Updating the crawler workflow also showed me that documentation must accurately reflect the implemented behaviour instead of the intended design.

While designing the Indexer components, I developed a clearer understanding of responsibility-driven design. I realised that HTML parsing, tokenization, and stop-word removal solve different problems and therefore require independent interfaces, processing rules, and failure handling. Documenting these boundaries before implementation provided a clear implementation roadmap and reduced the likelihood of tightly coupled code, making future components such as the Inverted Index easier to develop and extend.
