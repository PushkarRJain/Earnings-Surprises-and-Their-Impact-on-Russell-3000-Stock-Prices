#pragma once

#include <vector>
#include <map>
#include <bits/stdc++.h>
#include "DataFetcher.h"
#include "DataDownloader.h"

using namespace std;

class Bootstrapper {
private:
    int bootstrapIterations;
    int sampleSize;
    vector<vector<string>> MissStocks, BeatStocks, MeetStocks;
    unordered_map<string, vector<double>> BenchmarkMetrics;         
    vector<vector<vector<double>>> SurpriseMetrics3D;
    Bootstrapper(){}

public:
    Bootstrapper(int iterations = 40, int size = 30){
        bootstrapIterations = iterations;
        sampleSize = size;
    }
    ~Bootstrapper(){}

    void DoBootstrap(vector<string>& beatStocks, vector<string>& meetStocks, vector<string>& missStocks);
    vector<string> AggregateBootstrappStocks();
    void CalculateAllMetricsForAllGroups(DataDownloader fetcher);
    unordered_map<string,vector<double>> ShowGroupMetrics(const unordered_map<string, vector<double>> &metrics, const string &groupName);
    unordered_map<string,vector<double>> GetAllGroupsCAAR(const unordered_map<string, vector<double>> &metrics);

    // Getter Methods
    int GetBootstrapIterations(){
        return bootstrapIterations;
    }
    int GetSampleSize(){
        return sampleSize;
    }
    vector<vector<string>> GetMissGroup(){
        return MissStocks;
    }
    vector<vector<string>> GetBeatGroup(){
        return BeatStocks;
    }
    vector<vector<string>> GetMeetGroup(){
        return MeetStocks;
    }
    unordered_map<string, vector<double>> GetMetric(){
        return BenchmarkMetrics;
    }
};


