#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include "Plotter.h"
using namespace std;

namespace FinanceProject
{
void Plotter::plot_caar()
{
    int i = 0;
    int N = df->GetN();
    int nIntervals = 2*N+1;
    int dataSize=nIntervals;
    
    vector<double> xdata;
    for(i=-N;i<=N;i++){
        xdata.push_back(i);
    }
    
    auto miss=bs->GetMetric()["caar_mean_miss"];
    auto meet=bs->GetMetric()["caar_mean_meet"];
    auto beat=bs->GetMetric()["caar_mean_beat"];

    // Create temporary files first
    const char *tempDataFileName1 = "Miss";
    const char *tempDataFileName2 = "Meet";
    const char *tempDataFileName3 = "Beat";

    // Write data to files
    FILE *tempDataFile = fopen(tempDataFileName1,"w");
    if (tempDataFile) {
        for (i=0; i < dataSize; i++) {
            fprintf(tempDataFile,"%lf %lf\n",xdata[i],miss[i]);
        }
        fclose(tempDataFile);
    }

    tempDataFile = fopen(tempDataFileName2,"w");
    if (tempDataFile) {
        for (i=0; i < dataSize; i++) {
            fprintf(tempDataFile,"%lf %lf\n",xdata[i],meet[i]);
        }
        fclose(tempDataFile);
    }

    tempDataFile = fopen(tempDataFileName3,"w");
    if (tempDataFile) {
        for (i=0; i < dataSize; i++) {
            fprintf(tempDataFile,"%lf %lf\n",xdata[i],beat[i]);
        }
        fclose(tempDataFile);
    }

    // Now open gnuplot and create the plot
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe) {
        const char *title = "Average CAAR for Three Groups";
        const char *yLabel = "CAAR";

        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set title '%s'\n", title);
        fprintf(gnuplotPipe, "set arrow from 0.5,graph(0,0) to 0.5,graph(1,1) nohead lc rgb 'red'\n");
        fprintf(gnuplotPipe, "set xlabel 'Announcement Date'\nset ylabel '%s'\n", yLabel);
        fprintf(gnuplotPipe, "plot \"%s\" with lines title 'Miss', \"%s\" with lines title 'Meet', \"%s\" with lines title 'Beat'\n",
                tempDataFileName1, tempDataFileName2, tempDataFileName3);
        
        fflush(gnuplotPipe);
        
        printf("Press enter to continue...\n");
        getchar();
        
        pclose(gnuplotPipe);
        
        // Clean up temporary files
        remove(tempDataFileName1);
        remove(tempDataFileName2);
        remove(tempDataFileName3);
    } else {
        printf("Error: Could not open gnuplot pipe\n");
    }
}

}