#ifndef ARRAY_ASSIGNMENT_HPP
#define ARRAY_ASSIGNMENT_HPP

#include <string>



using namespace std;

struct Record {
    string customerID;
    string product;
    string category;
    double price;
    string date;
    string paymentMethod;

    int dateToInt() const {
        int day, month, year;
        sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);
        return year * 10000 + month * 100 + day;
    }
};

struct Review {
    string product_id;
    string customer_id;
    int rating;
    string review;
};

enum SortMode {
    BY_DATE,
    BY_CATEGORY,
    BY_PRICE
};




// Sorting
bool compareRecords(const Record& a, const Record& b, SortMode mode);
void bubbleSort(Record* arr, int size, SortMode mode);
void insertionSort(Record* arr, int size, SortMode mode);
void selectionSort(Record* arr, int size, SortMode mode);
void mergeSort(Record* arr, int left, int right, SortMode mode);

// Searching
int linearSearch(Record* arr, int size, const string& targetDate);
int binarySearch(Record* arr, int size, const string& target, SortMode mode);
int interpolationSearch(Record* arr, int size, const string& targetDate);
int jumpSearch(Record* arr, int size, const string& targetDate);

// Utilities
int readTransactionCSV(const string& filename, Record*& arr);
int readReviewCSV(const string& filename, Review*& arr);
void displayTransactions(Record* arr, int size);
void processElectronicsCreditCardPercentage(Record* transactions, int size);

// Review Processing
int filterReviews(Review*& reviews, int reviewCount, Record* transactions, int transCount);
void analyzeOneStarReviews(Review* reviews, int count);
void mergeSortR(Review* arr, int left, int right);

#endif // ARRAY_ASSIGNMENT_HPP
