cmake_minimum_required(VERSION 3.20)

project(Fetcher)

# ----------------------------
# C++ Settings
# ----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------
# Debug Build
# ----------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

include_directories(include)

# ----------------------------
# External Libraries
# ----------------------------
find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)

# ----------------------------
# Fetcher Library
# ----------------------------
add_library(fetcher
    src/Fetcher.cpp
    src/Frontier.cpp
    src/SeenStore.cpp
    src/PageStorage.cpp
    src/LinkExtractor.cpp
    src/URLNormalizer.cpp
    src/crawler.cpp
    src/Database.cpp
    ../../DS_Library/src/HashFunction.cpp
)

target_link_libraries(fetcher
    PUBLIC
        CURL::libcurl
        SQLite::SQLite3
)

# ----------------------------
# Main Executable
# ----------------------------
add_executable(testFetcher
    main.cpp
)

target_link_libraries(testFetcher
    PRIVATE
        fetcher
)

# ==========================================================
# Google Test (Disabled Temporarily)
# Uncomment this section when running unit tests.
# ==========================================================

enable_testing()

find_package(GTest REQUIRED)

add_executable(URLNormalizerTesting
    testing/URLNormalizerTest.cpp
    testing/LinkExtractorTest.cpp
    testing/FetcherTest.cpp
    testing/FrontierTest.cpp
    testing/SeenStoreTest.cpp
    testing/PageStorageTest.cpp
    testing/CrawlerTest.cpp
)

target_link_libraries(URLNormalizerTesting
    PRIVATE
        fetcher
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(URLNormalizerTesting)