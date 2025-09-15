# Earnings Surprises and Their Impact on Russell 3000 Stock Prices: A Quantitative Analysis

## Overview

This project implements a comprehensive C++ event study framework to analyze how quarterly earnings announcements impact stock performance within the Russell 3000 index. The system classifies stocks based on earnings surprise percentages (beat/meet/miss) and measures both Average Abnormal Returns (AAR) and Cumulative Average Abnormal Returns (CAAR) using statistical bootstrapping techniques.

## Key Features

- **Real-time Data Integration**: Automated market data retrieval using EODHD API
- **Statistical Robustness**: Monte Carlo bootstrapping for confidence interval estimation
- **Performance Optimization**: Multithreaded processing across earnings categories
- **Visual Analytics**: Automated CAAR visualization using gnuplot integration
- **Modular Architecture**: Object-oriented design with clean separation of concerns

## Architecture Overview

### Core Components

#### 1. DataFetcher
- **Purpose**: Central preprocessing module for CSV data validation and event window calculations
- **Key Features**:
  - Prompts user for window size (N)
  - Loads ticker symbols, earnings data, and trading calendar
  - Computes event window start/end dates based on trading calendar
  - Validates data integrity and handles missing values

#### 2. BootStrapper
- **Purpose**: Statistical resampling engine for robust AAR/CAAR estimation
- **Implementation**: Monte Carlo approach with configurable trial iterations
- **Output**: Confidence intervals and statistical significance testing
- **Method**: Extracts consistent random samples from each earnings category

#### 3. DataDownloader
- **Purpose**: Automated market data acquisition from external APIs
- **Technology**: libcurl integration with memory buffer management
- **Features**: 
  - Error handling and retry logic
  - Structured data parsing
  - Rate limiting compliance

#### 4. Menu System
- **Purpose**: User interface controller orchestrating all modules
- **Pattern**: MVC-like architecture
- **Functions**:
  - Data retrieval operations
  - Stock information queries
  - Metrics display and analysis
  - Visualization generation

#### 5. Plotter
- **Purpose**: Time-series visualization generator
- **Integration**: gnuplot external plotting system
- **Output**: CAAR graphs showing market reaction patterns
- **Features**: Customizable event window visualization

#### 6. Stock Module
- **Purpose**: Individual stock data processing and classification
- **Logic**: Earnings surprise analysis and group assignment
- **Integration**: Connects to global data maps for real-time classification

### Technical Implementation Details

#### Multithreading Architecture
```cpp
// Parallel processing across earnings categories
std::thread beat_thread(process_beat_stocks);
std::thread meet_thread(process_meet_stocks);
std::thread miss_thread(process_miss_stocks);

// Synchronization and result aggregation
beat_thread.join();
meet_thread.join();
miss_thread.join();
```

#### Operator Overloading
Custom implementations for:
- Vector arithmetic operations
- Stream formatting for containers
- Mathematical operations with error handling
- Dimension mismatch validation

#### Memory Management
- RAII principles throughout codebase
- Smart pointer usage for resource management
- Exception-safe operations
- Memory leak prevention

## Installation & Setup

### Prerequisites
```bash
# Required libraries
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install gnuplot
sudo apt-get install build-essential
```

### Compilation
```bash
# Standard compilation
g++ -std=c++17 -pthread -lcurl -o earnings_analysis *.cpp

# Debug build
g++ -std=c++17 -pthread -lcurl -g -DDEBUG -o earnings_analysis *.cpp

# Optimized release build
g++ -std=c++17 -pthread -lcurl -O3 -DNDEBUG -o earnings_analysis *.cpp
```

### Required Data Files
Place the following CSV files in the project directory:
- `russell3000_tickers.csv` - Russell 3000 constituent symbols
- `earnings_surprises.csv` - Quarterly earnings data with surprise percentages
- `trading_calendar.csv` - Valid trading dates for event window calculation

## Usage

### Basic Execution
```bash
./earnings_analysis
```

### Menu Options
1. **Load Data**: Initialize data structures and validate inputs
2. **Download Market Data**: Fetch latest price information via API
3. **Run Bootstrap Analysis**: Execute statistical resampling with user-defined parameters
4. **Generate CAAR Plots**: Create time-series visualizations
5. **Query Individual Stock**: Analyze specific ticker performance
6. **Export Results**: Save analysis outputs to CSV format

### Configuration Parameters
```cpp
// Configurable parameters in config.h
const int DEFAULT_WINDOW_SIZE = 20;    // Event window days
const int BOOTSTRAP_ITERATIONS = 1000; // Monte Carlo trials
const double SIGNIFICANCE_LEVEL = 0.05; // Statistical confidence
const int MAX_THREADS = 4;             // Parallel processing limit
```

## Methodology

### Event Study Framework
1. **Event Identification**: Quarterly earnings announcement dates
2. **Window Definition**: [-N, +N] days around announcement
3. **Abnormal Return Calculation**: 
   ```
   AR(i,t) = R(i,t) - E[R(i,t)]
   ```
4. **Statistical Aggregation**: Cross-sectional averaging with bootstrapping

### Earnings Classification
- **Beat**: Actual EPS > Consensus estimate
- **Meet**: Actual EPS ≈ Consensus estimate (within ±2%)
- **Miss**: Actual EPS < Consensus estimate

### Statistical Methods
- **Bootstrap Resampling**: 1000+ iterations for robust estimation
- **Confidence Intervals**: 95% significance testing
- **Cross-sectional Analysis**: Portfolio-level aggregation

## Results & Interpretation

### Key Findings from CAAR Analysis

#### Market Efficiency Validation
The time-series visualization demonstrates clear evidence of semi-strong form market efficiency:
- **Immediate Price Reaction**: Sharp divergence post-announcement
- **Information Incorporation**: Rapid adjustment to new earnings information
- **Persistence**: Sustained directional movement following surprises

#### Asymmetric Response Patterns
- **Positive Surprises**: Gradual upward momentum in beat stocks
- **Negative Surprises**: Sharp, persistent declines in miss stocks
- **Risk Aversion**: Stronger reactions to negative surprises suggest institutional repositioning

#### Trading Implications
- **Short-term Predictability**: Transient arbitrage opportunities
- **Momentum Effects**: Post-announcement drift patterns
- **Risk Management**: Asymmetric downside protection needs

### Sample Output
```
=== CAAR Analysis Results ===
Beat Stocks (N=250):
  Day -10: -0.12%
  Day 0:    2.34%
  Day +10:  4.67%

Miss Stocks (N=180):
  Day -10: -0.08%
  Day 0:   -3.21%
  Day +10: -5.89%

Statistical Significance: p < 0.001
```

## Performance Metrics

### Optimization Results
- **Baseline Processing**: 45 minutes (single-threaded)
- **Multithreaded Implementation**: 12 minutes (4 cores)
- **Memory Usage**: <2GB for full Russell 3000 analysis
- **API Rate Limiting**: 100 requests/minute compliance

### Scalability Considerations
- Horizontal scaling via additional threads
- Memory-mapped file I/O for large datasets
- Streaming data processing for real-time analysis
- Database integration for historical data storage

## Testing & Validation

### Unit Tests
```bash
# Run comprehensive test suite
./run_tests.sh

# Individual component testing
./test_bootstrap
./test_data_loader
./test_calculations
```

### Integration Testing
- End-to-end workflow validation
- API connectivity verification
- Data integrity checks
- Performance benchmarking

### Code Quality
- Static analysis with cppcheck
- Memory leak detection with valgrind
- Code coverage analysis
- Continuous integration pipeline

## API Integration

### EODHD API Usage
```cpp
// Example API call structure
std::string url = "https://eodhd.com/api/eod/" + ticker + 
                  ".US?api_token=" + API_KEY + 
                  "&from=" + start_date + "&to=" + end_date;
```

### Rate Limiting
- Implemented exponential backoff
- Request queuing system
- Error handling and retry logic
- API quota monitoring

## Contributing

### Development Setup
```bash
# Clone repository
git clone https://github.com/username/earnings-surprise-analysis.git
cd earnings-surprise-analysis

# Install development dependencies
sudo apt-get install valgrind cppcheck clang-format

# Setup pre-commit hooks
./setup_hooks.sh
```

### Code Standards
- Google C++ Style Guide compliance
- Comprehensive documentation requirements
- Unit test coverage >90%
- Performance regression testing

### Pull Request Process
1. Fork repository and create feature branch
2. Implement changes with full test coverage
3. Run static analysis and performance benchmarks
4. Submit PR with detailed description and test results

## License

MIT License - See LICENSE file for details

## References

- OANDA PropTrader Lab: Russell Index Analysis
- Capinski & Zastawniak: "Numerical Methods in Finance with C++"
- Duffy: "Introduction to C++ for Financial Engineers"
- NYU Tandon FRE6883 Course Materials

## Contact

**Project Team**: Koshiq Hossain, Aryan Singal, Ishit Bajpai, Pushkar Jain
**Course**: FRE-GY 6883 Financial Computing
**Institution**: NYU Tandon School of Engineering

For questions or collaboration opportunities, please open an issue or contact the development team.

---

*This project demonstrates advanced quantitative finance techniques combined with high-performance C++ implementation for institutional-grade financial analysis.*
