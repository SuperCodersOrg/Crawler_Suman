// #include <iostream>
// #include "../include/Crawler.h"

// using namespace std;

// int main()
// {
//     cout << "=========================================\n";
//     cout << "        PERSISTENT WEB CRAWLER\n";
//     cout << "=========================================\n\n";
//     int MAX_DEPTH;
//     cout<<"Enter max Depth: ";
//     cin>>MAX_DEPTH;
//     int MAX_PAGES;
//     cout<<"Enter max Pages: ";
//     cin>>MAX_PAGES;
//     Crawler crawler(MAX_DEPTH,MAX_PAGES);
//     int n;
//     cout<<"Enter no. of seedurls: ";
//     cin>>n;
//     for(int i=0;i<n;i++){
//         cout<<"Enter "<<(i+1)<<"th seed url: ";
//         string seedURL;
//         cin>>seedURL;
//         crawler.crawl(seedURL);
//         cout << "\n=========================================\n";
//         cout << "Crawler execution completed.\n";
//         cout << "=========================================\n";
//     }
//     // string seedURL =
//     //     "http://quotes.toscrape.com";

    
//     // const int MAX_PAGES = 40;

//     // cout << "Seed URL   : " << seedURL << endl;
//     // cout << "Max Depth  : " << MAX_DEPTH << endl;
//     // cout << "Max Pages  : " << MAX_PAGES << endl;
//     // cout << "\nStarting crawler...\n\n";

    

//     // crawler.crawl(seedURL);

    

//     return 0;
// }


// #include <iostream>

// #include "../include/crawler.h"
// #include "../include/PageStorage.h"

// #include "../include/HTMLParser.h"
// #include "../include/Tokenizer.h"
// #include "../include/StopWordRemover.h"
// #include "../include/InvertedIndex.h"

// using namespace std;

// int main()
// {
//     cout << "=========================================\n";
//     cout << "     CRAWLER + INDEXER INTEGRATION\n";
//     cout << "=========================================\n\n";

//     //--------------------------------------------------
//     // Crawl a few pages
//     //--------------------------------------------------

//     const int MAX_DEPTH = 1;
//     const int MAX_PAGES = 5;

//     Crawler crawler(MAX_DEPTH, MAX_PAGES);

//     crawler.crawl("https://quotes.toscrape.com");

//     cout << "\nCrawling completed.\n\n";

//     //--------------------------------------------------
//     // Create Indexer Components
//     //--------------------------------------------------

//     HTMLParser parser;
//     Tokenizer tokenizer;
//     StopWordRemover remover;
//     InvertedIndex index;

//     PageStorage storage;

//     //--------------------------------------------------
//     // Index every downloaded page
//     //--------------------------------------------------

//     for(int pageID = 1; pageID <= storage.pageCount(); pageID++)
//     {
//         cout << "Indexing Page " << pageID << endl;
//         string url=storage.getURLByID(pageID);
//         string html = storage.getPage(url);
//         DynamicArray<string> text = parser.extractText(html);
//         for(int  i=0;i<text.getSize();i++){
//             cout<<text[i]<<endl;
//         }
//         DynamicArray<string> tokens = tokenizer.tokenize(text);

//         DynamicArray<string> filtered = remover.removeStopWords(tokens);
//         cout<<"Everything is done: "<<endl;
//         for(int i=0;i<filtered.getSize();i++){
//             cout<<filtered[i]<<" ";
//         }
//         cout<<endl;
//         for(int i = 0; i < filtered.getSize(); i++)
//         {
//             index.insert(filtered[i], pageID);
//         }
//     }

//     cout << "\n=========================================\n";
//     cout << "Indexing Completed\n";
//     cout << "=========================================\n\n";

//     cout << "Vocabulary Size : "
//          << index.vocabularySize()
//          << endl;

//     //--------------------------------------------------
//     // Display postings for a few words
//     //--------------------------------------------------

//     DynamicArray<string> queries;

//     queries.push_back("life");
//     queries.push_back("truth");
//     queries.push_back("love");
//     queries.push_back("world");
//     queries.push_back("people");

//     cout << "\n========== Sample Search ==========\n\n";

//     for(int i = 0; i < queries.getSize(); i++)
//     {
//         cout << queries[i] << " -> ";

//         if(!index.contains(queries[i]))
//         {
//             cout << "Not Found\n";
//             continue;
//         }

//         DynamicArray<Posting>& postings = index.getPostings(queries[i]);

//        for(int j = 0; j < postings.getSize(); j++)
//         {   cout<<"\nFileName    :"
//                 <<postings[j].pageID<<".page";
//             cout << "\nURL       : "
//                 << storage.getURLByID(postings[j].pageID)
//                 << endl;
//             cout << "Frequency : "
//                 << postings[j].frequency
//                 << endl;
//         }

//         cout << endl;
//     }

//     return 0;
// }




#include <iostream>
#include <stdexcept>

#include "../include/Crawler.h"
#include "../include/Indexer.h"

using namespace std;

int main()
{
    try
    {
        //--------------------------------------------------
        // Crawl
        //--------------------------------------------------

        cout << "===== Crawler =====" << endl;

        Crawler crawler(1,10);
        crawler.crawl("https://quotes.toscrape.com");

        //--------------------------------------------------
        // Build Index
        //--------------------------------------------------

        cout << "\n===== Indexer =====" << endl;

        Indexer indexer;

        indexer.buildIndex();

        cout << "Index built successfully." << endl;

        //--------------------------------------------------
        // Search
        //--------------------------------------------------

        while(true)
        {
            string word;

            cout << "\nEnter word to search (type 'exit' to quit): ";
            cin >> word;

            if(word == "exit")
            {
                break;
            }

            DynamicArray<string> urls = indexer.search(word);

            if(urls.isEmpty())
            {
                cout << "No pages found for \"" << word << "\"." << endl;
                continue;
            }

            cout << "\nFound in " << urls.getSize()
                 << " page(s):" << endl;

            for(int i = 0; i < urls.getSize(); i++)
            {
                cout << i + 1 << ". "
                     << urls[i]
                     << endl;
            }
        }

        cout << "\nProgram terminated." << endl;
    }
    catch(const exception& e)
    {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}