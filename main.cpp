#include <iostream>
#include "../include/Crawler.h"

using namespace std;

int main()
{
    cout << "=========================================\n";
    cout << "        PERSISTENT WEB CRAWLER\n";
    cout << "=========================================\n\n";

    const string seedURL =
        "http://quotes.toscrape.com";

    const int MAX_DEPTH = 2;
    const int MAX_PAGES = 40;

    cout << "Seed URL   : " << seedURL << endl;
    cout << "Max Depth  : " << MAX_DEPTH << endl;
    cout << "Max Pages  : " << MAX_PAGES << endl;
    cout << "\nStarting crawler...\n\n";

    Crawler crawler(
        MAX_DEPTH,
        MAX_PAGES);

    crawler.crawl(seedURL);

    cout << "\n=========================================\n";
    cout << "Crawler execution completed.\n";
    cout << "=========================================\n";

    return 0;
}