#include <bits/stdc++.h>
#include <chrono>
#include "Menu.h"

#include "../Include/DataFetcher.h"
#include "../Include/DataDownloader.h"
#include "../Include/DataSorting.h"
#include "../Include/Bootstrapper.h"
#include "../Include/Plotter.h"
#include "../Include/Stock.h"
#include "../Include/OperatorOverloads.h"

using namespace std;
using namespace FinanceProject;

// ANSI color codes
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";

Menu::Menu(int initChoice) : choice(initChoice), fetcher(),
      downloader(),  
      bootstrapper(40, 30) { 
    DisplayMenu();
    ProcessChoice();
}

void Menu::DisplayMenu() {
    cout << "\n" << string(60, '=') << "\n";
    cout << BOLD << CYAN << "                FRE6883 Final Project Menu                " << RESET << "\n";
    cout << string(60, '=') << "\n\n";
    
    cout << BOLD << "Available Options:" << RESET << "\n";
    cout << GREEN << "1. " << RESET << "Retrieve Historical Price Data\n";
    cout << GREEN << "2. " << RESET << "Pull Stock Information\n";
    cout << GREEN << "3. " << RESET << "Display AAR and CAAR Metrics\n";
    cout << GREEN << "4. " << RESET << "Show CAAR Graph\n";
    cout << RED << "5. " << RESET << "Exit\n";
    
    cout << "\n" << YELLOW << "Note: " << RESET << "Please ensure you have completed step 1 before proceeding with other options.\n";
    cout << string(60, '=') << "\n";
}

void Menu::ProcessChoice() {
    do {
        cout << "\n" << BOLD << "Enter your choice (1-5): " << RESET;
        cin >> choice;

        switch (choice) {
        case '1':
            cout << "\n" << BLUE << "=== Retrieving Historical Price Data ===" << RESET << "\n";
            fetcher.enterN();
            RetrieveHistoricalPriceData();
            break;
        case '2':
            cout << "\n" << BLUE << "=== Pulling Stock Information ===" << RESET << "\n";
            PullStockInformation();
            break;
        case '3':
            cout << "\n" << BLUE << "=== Displaying AAR and CAAR Metrics ===" << RESET << "\n";
            DisplayAARAndCAARMetrics();
            break;
        case '4':
            cout << "\n" << BLUE << "=== Showing CAAR Graph ===" << RESET << "\n";
            ShowCAARGraph();
            break;
        case '5':
            cout << "\n" << MAGENTA << "Thank you for using the Financial Computing Program!" << RESET << "\n";
            cout << "Exiting the program. Goodbye!\n";
            break;
        default:
            cout << "\n" << RED << "Invalid choice. Please enter a number between 1 and 5." << RESET << "\n";
        }

        if (choice != '5') {
            DisplayMenu();
        }

    } while (choice != '5');
}

void Menu::RetrieveHistoricalPriceData() {
    using namespace std::chrono;

    auto start_time = high_resolution_clock::now();

    cout << "[LOG] Sorting stocks into groups..." << endl;
    SortingGroup sorter(fetcher.GetEarningsDataMap());
    sorter.sort_group();

    vector<string> beatStocks = sorter.getBeat();
    vector<string> meetStocks = sorter.getMeet();
    vector<string> missStocks = sorter.getMiss();

    auto allStocks = beatStocks;
    allStocks.insert(allStocks.end(), meetStocks.begin(), meetStocks.end());
    allStocks.insert(allStocks.end(), missStocks.begin(), missStocks.end());

    cout << "[LOG] Downloading stock data for all grouped tickers..." << endl;
    // Multithreaded data download
    int n = 3;
    int group_size = allStocks.size() / n;
    vector<vector<string>> stock_groups(n);
    for (int i = 0; i < n; ++i) {
        int start = i * group_size;
        int end = (i == n - 1) ? allStocks.size() : (i + 1) * group_size;
        stock_groups[i] = vector<string>(allStocks.begin() + start, allStocks.begin() + end);
    }

    auto& a = downloader.GetStockDataMap();
    int b = fetcher.GetN();
    auto c = fetcher.GetEarningsDates();
    auto d = fetcher.GetEarningsDatesStartMap();
    auto e = fetcher.GetEarningsDatesEndMap();

    thread t1([&]() { downloader.fetchDataFromEod(stock_groups[0], a, b, c, d, e); });
    thread t2([&]() { downloader.fetchDataFromEod(stock_groups[1], a, b, c, d, e); });
    thread t3([&]() { downloader.fetchDataFromEod(stock_groups[2], a, b, c, d, e); });

    t1.join(); t2.join(); t3.join();

    cout << "[LOG] Filtering stocks with valid downloaded data..." << endl;

    // Filter tickers by valid data
    auto& stockDataMap = downloader.GetStockDataMap();
    auto isValid = [&](const string& symbol) {
        return stockDataMap.find(symbol) != stockDataMap.end() && !stockDataMap[symbol].empty();
    };

    vector<string> filteredBeat, filteredMeet, filteredMiss;
    for (auto& s : beatStocks) if (isValid(s)) filteredBeat.push_back(s);
    for (auto& s : meetStocks) if (isValid(s)) filteredMeet.push_back(s);
    for (auto& s : missStocks) if (isValid(s)) filteredMiss.push_back(s);

    cout << "[LOG] Valid beat: " << filteredBeat.size() << ", meet: " << filteredMeet.size()
         << ", miss: " << filteredMiss.size() << endl;

    // Only now perform bootstrap
    bootstrapper.DoBootstrap(filteredBeat, filteredMeet, filteredMiss);

    // Then compute metrics
    bootstrapper.CalculateAllMetricsForAllGroups(downloader);

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time).count();
    cout << "Time taken to execute RetrieveHistoricalPriceData: " << duration << " milliseconds." << endl;
}

void Menu::PullStockInformation() {
    cout << "\n" << YELLOW << "Enter the stock symbol (e.g., AAPL): " << RESET;
    string stock_symbol;
    cin >> stock_symbol;
    cout << "\n" << BLUE << "Fetching information for " << BOLD << stock_symbol << RESET << "...\n\n";
    Stock mystock(stock_symbol,&fetcher);
    mystock.GetEveryInfo();
}

void Menu::DisplayAARAndCAARMetrics() {
    cout << "\n" << YELLOW << "Available Groups:" << RESET << "\n";
    cout << "1. Beat (Stocks that beat earnings)\n";
    cout << "2. Meet (Stocks that met earnings)\n";
    cout << "3. Miss (Stocks that missed earnings)\n\n";

    string groupName = "";
    while (groupName != "miss" && groupName != "meet" && groupName != "beat") {
        cout << YELLOW << "Enter group name (miss/meet/beat): " << RESET;
        cin >> groupName;
        transform(groupName.begin(), groupName.end(), groupName.begin(), ::tolower);
    }
    cout << "\n" << BLUE << "Displaying metrics for " << BOLD << groupName << RESET << " group:\n\n";

    unordered_map<string, vector<double>> metricsResult = bootstrapper.ShowGroupMetrics(bootstrapper.GetMetric(), groupName);

    cout << BOLD << "Average AARs:" << RESET << "\n" << metricsResult["aar_mean"] << "\n\n";
    cout << BOLD << "AAR Standard Deviation:" << RESET << "\n" << metricsResult["aar_std"] << "\n\n";
    cout << BOLD << "Average CAARs:" << RESET << "\n" << metricsResult["caar_mean"] << "\n\n";
    cout << BOLD << "CAAR Standard Deviation:" << RESET << "\n" << metricsResult["caar_std"] << "\n";
}

void Menu::ShowCAARGraph() {
    cout << "\n" << BLUE << "Generating CAAR graph..." << RESET << "\n";
    Plotter myplotter(&fetcher,&bootstrapper);
    myplotter.plot_caar();
    cout << GREEN << "Graph has been generated successfully!" << RESET << "\n";
}