# Build Log - Session 2

**Date:** 24-07-2026  
**Duration:** 2 hours

### Goal

Complete unit testing for the persistence layer of the crawler by validating the **PageStorage** and **SeenStore** components. Following a discussion with the project tutor, complete the remaining design documentation for the crawler and update the crawler workflow to accurately reflect the implemented persistence mechanism.

---

### Problem

Although the crawler's persistence layer had been implemented, it required thorough testing to ensure that page storage, crawler recovery, and completed URL management functioned correctly across multiple executions.

Additionally, during the project review with the tutor, it was observed that three important components—**Fetcher**, **LinkExtractor**, and **URLNormalizer**—were implemented but did not yet have formal design proposals. Since these components are fundamental to the crawler pipeline, the documentation was incomplete.

While reviewing the crawler workflow, it also became necessary to document the actual recovery behaviour implemented in the code so that the design proposal accurately matched the implementation.

---

### What I Tried

#### 1. PageStorage Testing

Developed comprehensive GoogleTest cases for the **PageStorage** component to validate both filesystem and database operations.

The implemented tests verified:

- Initial empty storage state.
- Adding pending URLs to the persistent database.
- Storing crawled pages successfully.
- Retrieving previously stored HTML pages.
- Handling requests for non-existent pages.
- Storing multiple pages correctly.
- Allowing storage of empty HTML content.
- Creation of persistent `.page` files inside the crawler storage directory.
- Recovery of crawler state after program restart.
- Correct URL retrieval using stored page IDs.

The tests ensured that the storage layer correctly synchronizes the database with the filesystem while maintaining crawler metadata.

---

#### 2. SeenStore Testing

Implemented GoogleTest cases for the **SeenStore** component to verify management of completed URLs.

The test suite covered:

- Empty store initialization.
- URL insertion.
- Duplicate URL prevention.
- Storage of multiple completed URLs.
- URL existence checking.
- Clearing the completed URL set.

These tests confirmed that duplicate completed pages are never inserted multiple times and that lookup operations remain consistent throughout crawler execution.

---

#### 3. Completed Remaining Design Proposals

After discussing the crawler architecture with the project tutor, prepared the remaining design proposals for:

- **Fetcher**
- **LinkExtractor**
- **URLNormalizer**

Each design proposal was expanded to include:

- Component responsibilities.
- Public interface.
- Internal workflow.
- Interaction with other crawler components.
- Error handling.
- Time and space complexity.
- Design rationale.

This completed the design documentation for all major crawler components.

---

#### 4. Updated Persistent Crawler Workflow Documentation

While reviewing the implementation, updated the crawler workflow documentation to reflect the actual persistence behaviour implemented in the source code.

The workflow now documents that:

- The crawler first normalizes the supplied seed URL.
- It checks whether the seed URL already exists in persistent storage using `storage.hasPage(seedURL)`.
- If the seed URL has previously been crawled, the crawler invokes `initializeCrawler()`, which restores the **Frontier** and **SeenStore** from persistent storage through `recoverCrawlerState()`.
- If the seed URL is not present in storage, no recovery is performed and the crawler begins a completely new crawl with empty Frontier and SeenStore instances.

---

### Outcome

Successfully completed comprehensive testing of the **PageStorage** and **SeenStore** components, validating the crawler's persistence and recovery mechanisms.

The remaining design proposals for **Fetcher**, **LinkExtractor**, and **URLNormalizer** were completed following the tutor's feedback, resulting in documentation for every major crawler component.

