#ifndef OPERATOR_OVERLOADS_H
#define OPERATOR_OVERLOADS_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class OperatorOverloads {
public:
    // Vector subtraction operator
    static vector<double> VectorSubtraction(const vector<double>& lhs, const vector<double>& rhs);
    
    // Vector output operator
    static ostream& VectorOutput(ostream& os, const vector<double>& vec);
    
    // Map output operator
    static ostream& MapOutput(ostream& os, const map<string, double>& st);
};

// Actual operator overloads that will call the static methods
vector<double> operator-(const vector<double>& lhs, const vector<double>& rhs);
ostream& operator<<(ostream& os, const vector<double>& vec);
ostream& operator<<(ostream& os, const map<string, double>& st);

#endif // OPERATOR_OVERLOADS_H