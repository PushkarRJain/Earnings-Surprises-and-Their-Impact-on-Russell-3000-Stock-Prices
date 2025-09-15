#include <OperatorOverloads.h>
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

// Vector subtraction implementation
vector<double> OperatorOverloads::VectorSubtraction(const vector<double>& lhs, const vector<double>& rhs) {
    if (lhs.size() != rhs.size()) {
        throw std::invalid_argument("Vectors must be the same size for subtraction.");
    }

    vector<double> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); i++) {
        result[i] = lhs[i] - rhs[i];
    }
    return result;
}

// Vector output implementation
ostream& OperatorOverloads::VectorOutput(ostream& os, const vector<double>& vec) {
    const int valuesPerLine = 10;
    int count = 0;
    
    for (auto val : vec) {
        os << setw(8) << val << " ";
        count++;
        if (count % valuesPerLine == 0) os << "\n";
    }

    if (vec.size() % valuesPerLine != 0) {
        os << "\n";
    }

    return os;
}

// Map output implementation
ostream& OperatorOverloads::MapOutput(ostream& os, const map<string, double>& st) {
    std::vector<std::string> dates;
    std::vector<double> prices;

    for (auto &kv : st) {
        dates.push_back(kv.first);
        prices.push_back(kv.second);
    }

    int columns = 11;
    int width = 14;

    {
        int i = 0;
        for (auto &d : dates) {
            os << std::setw(width) << d;
            i++;
            if (i % columns == 0) os << "\n";
        }
        if (!dates.empty() && dates.size() % columns != 0) os << "\n";
    }

    os << "====================================" << std::endl;
    
    {
        int i = 0;
        for (auto &p : prices) {
            os << std::setw(width) << std::fixed << std::setprecision(3) << p;
            i++;
            if (i % columns == 0) os << "\n";
        }
        if (!prices.empty() && prices.size() % columns != 0) os << "\n";
    }

    return os;
}

// Actual operator overloads that call the static methods
vector<double> operator-(const vector<double>& lhs, const vector<double>& rhs) {
    return OperatorOverloads::VectorSubtraction(lhs, rhs);
}

ostream& operator<<(ostream& os, const vector<double>& vec) {
    return OperatorOverloads::VectorOutput(os, vec);
}

ostream& operator<<(ostream& os, const map<string, double>& st) {
    return OperatorOverloads::MapOutput(os, st);
}