#ifndef LINKED_ASSIGNMENT_HPP
#define LINKED_ASSIGNMENT_HPP

#include <string>
#include <unordered_map>

using namespace std;

// ---------------- Structs ----------------

struct Record {
    string customerID;
    string product;
    string category;
    double price;
    string date;
    string paymentMethod;

    int dateToInt() const {
        int month, day, year;
        sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);
        return year * 10000 + month * 100 + day;
    }
};

struct TransactionNode {
    Record data;
    TransactionNode* next;

    TransactionNode(Record record) : data(record), next(nullptr) {}
};

struct ReviewNode {
    string product_id;
    string customer_id;
    int rating;
    string review;
    ReviewNode* link;
};

// ---------------- Transaction Functions ----------------

TransactionNode* createTransactionNode(const Record& record);
void appendTransactionNode(TransactionNode*& head, TransactionNode* newNode);
TransactionNode* readTransactionCSV(const string& filename);

// ---------------- Sorting Algorithms ----------------

TransactionNode* bubbleSort(TransactionNode* head);
TransactionNode* selectionSort(TransactionNode* head);
TransactionNode* insertionSort(TransactionNode*& head);
TransactionNode* merge(TransactionNode* left, TransactionNode* right);
TransactionNode* mergeSort(TransactionNode* head);

// ---------------- Searching Algorithms ----------------

void linearSearchByDate(TransactionNode* head, const string& targetDate);
TransactionNode* binarySearch(TransactionNode* head, const string& targetDate);
void binarySearchByDate(TransactionNode* head, const string& targetDate);
int dateToInt(const string& date);
void jumpSearchByDate(TransactionNode* head, const string& targetDate);
void interpolationSearchByDate(TransactionNode* head, const string& targetDate);

// ---------------- Transaction Utilities ----------------

void displayTransactions(TransactionNode* head);
TransactionNode* getNodeAtPosition(TransactionNode* head, int pos);
int getListLength(TransactionNode* head);
void processElectronicsCreditCardPercentage(TransactionNode* head);

// ---------------- Review Functions ----------------

ReviewNode* createReviewNode(const string& product_id, const string& customer_id, int rating, const string& review);
void appendReviewNode(ReviewNode** head, ReviewNode* node);
void displayReviews(ReviewNode* head);
int countReviews(ReviewNode* head);
void filterReviews(ReviewNode** reviewHeadRef, TransactionNode* transactionHead, bool debug);
void saveReviewsToCSV(ReviewNode* head, const string& filename);

// ---------------- Review Analysis ----------------

string cleanWord(const string& word);
ReviewNode* mergeByReviewLength(ReviewNode* a, ReviewNode* b);
ReviewNode* mergeSortByReviewLength(ReviewNode* head);
void displayWordFrequenciesInOneStarReviews(ReviewNode* head);

#endif // LINKED_ASSIGNMENT_HPP
