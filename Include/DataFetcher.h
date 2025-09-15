#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <EarningsData.h>
using namespace std;

typedef vector<double> Vector;
typedef vector<Vector> Matrix;
typedef map<string, double> Stock_data; //<date, return>


class DataFetcher {
private:
    unsigned N;
    string componentStocksFile;
    string earningsAnnouncementsFile;
    string tradingCalendarFile;

    vector<string> stockSymbols;
    vector<string> earningsDates;
    vector<string> tradingDates;
    vector<string> allTickers;

    map<string, string> earningsDatesStartMap;
    map<string, string> earningsDatesEndMap;
    map<string, earningsData> earningsDataMap;

public:
    DataFetcher() {}

    void enterN();
    void readAndStoreTickers(vector<string>& allTickers);
    void readAndStoreEarningsData(vector<string>& symbols, vector<string>& dates, const vector<string>& allTickers);
    void retrieveTradingDates(vector<string>& dates);
    void calculateStartAndEndDateForDataFetch(map<string, string>& startDateMap, map<string, string>& endDateMap);

    // Getters
    int GetN() { return N; }
    vector<string>& GetEarningsDates() { return earningsDates; }
    vector<string>& GetTradingDates() { return tradingDates; }
    map<string, string>& GetEarningsDatesStartMap() { return earningsDatesStartMap; }
    map<string, string>& GetEarningsDatesEndMap() { return earningsDatesEndMap; }
    map<string, earningsData>& GetEarningsDataMap() { return earningsDataMap; }

    ~DataFetcher() {}
};