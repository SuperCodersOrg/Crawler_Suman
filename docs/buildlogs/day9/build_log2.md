# Build Log — Session 2

**Date:** July 29, 2026

**Duration:** 2 hours

**Goal:**
Develop comprehensive test cases for the Indexer subsystem and validate the integration of all indexing components.

**Problem:**
Although the implementation of the Indexer module was complete, the functionality of each component needed to be verified independently before validating the complete indexing workflow.

The challenge was designing test cases that covered both normal and edge-case scenarios. Individual components such as `HTMLParser`, `Tokenizer`, `StopWordRemover`, `InvertedIndex`, and `IndexStorage` required unit testing, while the `Indexer` required integration testing because it coordinates multiple components.

Another challenge was ensuring that the tests remained independent and verified the expected behaviour without depending on previously executed test cases.

**What I Tried:**

* Reviewed the functionality of each Indexer component and identified the behaviours that required validation.
* Designed Google Test unit test cases for:

  * `HTMLParser`

    * extraction of visible text,
    * handling empty HTML,
    * missing body tags,
    * malformed HTML.
  * `Tokenizer`

    * word tokenization,
    * lowercase conversion,
    * punctuation handling,
    * empty input.
  * `StopWordRemover`

    * removal of predefined stop words,
    * preservation of meaningful words,
    * empty token list.
  * `InvertedIndex`

    * insertion of new words,
    * frequency updates,
    * retrieval of posting lists,
    * searching for missing terms.
  * `IndexStorage`

    * initialization,
    * saving posting records,
    * loading stored postings,
    * clearing stored data.
* Implemented an integration test for the `Indexer` to verify the complete indexing pipeline from stored HTML pages to searchable URLs.
* Executed the test suite and verified that all implemented components interacted correctly within the indexing workflow.

The testing hierarchy became:

```text
                 Google Test

        +-----------+-----------+

        |           |           |

        ▼           ▼           ▼

   Unit Tests   Integration Tests

        |                |

        ▼                ▼

Individual Components   Indexer Workflow
```

**Outcome:**

Successfully developed a comprehensive testing suite for the Indexer subsystem.

The unit tests verified the correctness of individual components, while the integration test confirmed that the complete indexing pipeline functioned as expected. These tests improved confidence in the implementation and provided a foundation for validating future modifications to the indexing system.

---

**Git Commits:**

**Commit ID:** `f689520`

**Commit Message:**

```text
Add unit tests for Indexer Components
```

**Commit ID:** `0a7eeba`

**Commit Message:**

```text
Add Intergration test for Indexer
```
