#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

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


// Sorting Modes
enum SortMode {
    BY_DATE,
    BY_CATEGORY,
    BY_PRICE
};

bool compareRecords(const Record& a, const Record& b, SortMode mode) {
    switch (mode) {
        case BY_DATE:
            return a.dateToInt() < b.dateToInt();
        case BY_CATEGORY:
            return a.category < b.category;
        case BY_PRICE:
            return a.price < b.price;
        default:
            return false;
    }
}

// Bubble Sort
void bubbleSort(Record* arr, int size, SortMode mode) {
    for (int i = 0; i < size - 1; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (!compareRecords(arr[j], arr[j + 1], mode)) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Insertion Sort
void insertionSort(Record* arr, int size, SortMode mode) {
    for (int i = 1; i < size; ++i) {
        Record key = arr[i];
        int j = i - 1;
        while (j >= 0 && !compareRecords(arr[j], key, mode)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Selection Sort
void selectionSort(Record* arr, int size, SortMode mode) {
    for (int i = 0; i < size - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < size; ++j) {
            if (compareRecords(arr[j], arr[minIdx], mode)) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
        }
    }
}

// Merge Sort
void merge(Record* arr, int left, int mid, int right, SortMode mode) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Record* L = new Record[n1];
    Record* R = new Record[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareRecords(L[i], R[j], mode)) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(Record* arr, int left, int right, SortMode mode) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, mode);
        mergeSort(arr, mid + 1, right, mode);
        merge(arr, left, mid, right, mode);
    }
}


// Searching Algorithms
int linearSearch(Record* arr, int size, const string& targetDate) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].date == targetDate) {
            return i;  // Return the index of the first match
        }
    }
    return -1;  // No match found
}

int binarySearch(Record* arr, int size, const string& target, SortMode mode) {
    int left = 0, right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        switch (mode) {
            case BY_DATE: {
                int midVal = arr[mid].dateToInt();
                int targetVal = Record{.date = target}.dateToInt();
                if (midVal == targetVal) return mid;
                else if (midVal < targetVal) left = mid + 1;
                else right = mid - 1;
                break;
            }
            case BY_CATEGORY: {
                if (arr[mid].category == target) return mid;
                else if (arr[mid].category < target) left = mid + 1;
                else right = mid - 1;
                break;
            }
            default:
                return -1;
        }
    }

    return -1;
}


int interpolationSearch(Record* arr, int size, const string& targetDate) {
    int lo = 0, hi = size - 1;
    int targetInt = Record{.date = targetDate}.dateToInt();

    while (lo <= hi && targetInt >= arr[lo].dateToInt() && targetInt <= arr[hi].dateToInt()) {
        if (lo == hi) {
            if (arr[lo].dateToInt() == targetInt) return lo;
            return -1;
        }
        int pos = lo + ((double)(hi - lo) / (arr[hi].dateToInt() - arr[lo].dateToInt())) *
                         (targetInt - arr[lo].dateToInt());

        if (arr[pos].dateToInt() == targetInt) return pos;
        if (arr[pos].dateToInt() < targetInt) lo = pos + 1;
        else hi = pos - 1;
    }
    return -1;
}

int jumpSearch(Record* arr, int size, const string& targetDate) {
    int targetInt = Record{.date = targetDate}.dateToInt();
    int step = sqrt(size);
    int prev = 0;

    while (prev < size && arr[min(step, size) - 1].dateToInt() < targetInt) {
        prev = step;
        step += sqrt(size);
        if (prev >= size) return -1;
    }

    for (int i = prev; i < min(step, size); ++i) {
        if (arr[i].dateToInt() == targetInt) return i;
    }

    return -1;
}

// Utility Functions
void processElectronicsCreditCardPercentage(Record* transactions, int size) {
    if (size == 0 || transactions == nullptr) {
        cout << "No transactions found." << endl;
        return;
    }

    // Step 1: Sort by category
    mergeSort(transactions, 0, size - 1, BY_CATEGORY);
    cout << "\n=== ELECTRONICS CATEGORY PAYMENT ANALYSIS ===\n";

    // Step 2: Binary search using SortMode
    int index = binarySearch(transactions, size, "Electronics", BY_CATEGORY);
    if (index == -1) {
        cout << "No transactions in Electronics category.\n";
        return;
    }

    // Step 3: Expand left/right to gather all matching entries
    int left = index - 1;
    while (left >= 0 && transactions[left].category == "Electronics") left--;
    int right = index + 1;
    while (right < size && transactions[right].category == "Electronics") right++;

    // Step 4: Count total + credit card payments
    int totalElectronics = 0;
    int creditCardElectronics = 0;
    for (int i = left + 1; i < right; ++i) {
        totalElectronics++;
        if (transactions[i].paymentMethod == "Credit Card") {
            creditCardElectronics++;
        }
    }

    // Step 5: Output
    double percentage = (totalElectronics > 0)
        ? (static_cast<double>(creditCardElectronics) / totalElectronics) * 100.0
        : 0.0;

    cout << "Total Electronics Transactions: " << totalElectronics << endl;
    cout << "Electronics transactions paid via Credit Card: " << creditCardElectronics << endl;
    cout << "Percentage of Electronics purchases made using Credit Card: "
         << fixed << setprecision(2) << percentage << "%\n";
}



string cleanWord(const string& word) {
    string cleaned;
    for (char c : word) {
        if (isalnum(c)) cleaned += tolower(c);
    }
    return cleaned;
}

int readTransactionCSV(const string& filename, Record*& arr) {
    ifstream file(filename);
    string line;
    int count = 0;
    int capacity = 100;
    arr = new Record[capacity];

    if (!file.is_open()) {
        cerr << "Error: Could not open transaction file." << endl;
        return 0;
    }

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;
        getline(ss, customerID, ',');
        getline(ss, product, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, date, ',');
        getline(ss, paymentMethod, ',');

        if (count == capacity) {
            capacity *= 2;
            Record* newArr = new Record[capacity];
            for (int i = 0; i < count; ++i) newArr[i] = arr[i];
            delete[] arr;
            arr = newArr;
        }

        arr[count++] = {customerID, product, category, stod(priceStr), date, paymentMethod};
    }

    file.close();
    return count;
}

int readReviewCSV(const string& filename, Review*& arr) {
    ifstream file(filename);
    string line;
    int count = 0;
    int capacity = 100;
    arr = new Review[capacity];

    if (!file.is_open()) {
        cerr << "Error: Could not open review file." << endl;
        return 0;
    }

    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string product_id, customer_id, ratingStr, review;
        getline(ss, product_id, ',');
        getline(ss, customer_id, ',');
        getline(ss, ratingStr, ',');
        getline(ss, review); 

        if (count == capacity) {
            capacity *= 2;
            Review* newArr = new Review[capacity];
            for (int i = 0; i < count; ++i) newArr[i] = arr[i];
            delete[] arr;
            arr = newArr;
        }

        arr[count++] = {product_id, customer_id, stoi(ratingStr), review};
    }

    file.close();
    return count;
}

void displayTransactions(Record* arr, int size) {
    for (int i = 0; i < size; ++i) {
        cout << "Customer ID: " << arr[i].customerID << "\n";
        cout << "Product: " << arr[i].product << "\n";
        cout << "Category: " << arr[i].category << "\n";
        cout << "Price: $" << arr[i].price << "\n";
        cout << "Date: " << arr[i].date << "\n";
        cout << "Payment Method: " << arr[i].paymentMethod << "\n\n";
    }
}

int filterReviews(Review*& reviews, int reviewCount, Record* transactions, int transCount) {
    unordered_map<string, int> transactionCounts;
    for (int i = 0; i < transCount; ++i) {
        transactionCounts[transactions[i].customerID]++;
    }

    unordered_map<string, int> reviewCounts;
    Review* filtered = new Review[reviewCount];
    int validCount = 0;

    for (int i = 0; i < reviewCount; ++i) {
        string cid = reviews[i].customer_id;
        if (reviewCounts[cid] < transactionCounts[cid]) {
            filtered[validCount++] = reviews[i];
            reviewCounts[cid]++;
        }
    }

    delete[] reviews;
    reviews = filtered;
    return validCount;
}

void analyzeOneStarReviews(Review* reviews, int count) {
    unordered_map<string, int> wordFreq;

    for (int i = 0; i < count; ++i) {
        if (reviews[i].rating == 1) {
            stringstream ss(reviews[i].review);
            string word;
            while (ss >> word) {
                word = cleanWord(word);
                if (!word.empty()) wordFreq[word]++;
            }
        }
    }

    if (wordFreq.empty()) {
        cout << "No 1-star reviews found.\n";
        return;
    }

    multimap<int, string, greater<int>> sortedWords;
    for (const auto& pair : wordFreq) {
        sortedWords.insert({pair.second, pair.first});
    }

    cout << "\nWord Frequencies in 1-Star Reviews:\n";
    for (const auto& [freq, word] : sortedWords) {
        cout << word << ": " << freq << endl;
    }
}


// // Q1 FULL
// int main() {
//     // Read the transaction data from the CSV file
//     Record* transactions;
//     int transactionCount = readTransactionCSV("transactions_cleaned.csv", transactions);
//     if (transactionCount == 0) {
//         cerr << "Failed to load transaction data." << endl;
//         return 1;
//     }

//     // === SORT METHOD ===
//     mergeSort(transactions, 0, transactionCount - 1, BY_DATE);

//     // === SEARCH TARGET INPUT ===
//     string targetDate;
//     cout << "Enter date to search (format DD/MM/YYYY): ";
//     getline(cin, targetDate);

//     auto start = high_resolution_clock::now();

//     // === SEARCH METHOD ===
//     // int index = binarySearch(transactions, transactionCount, targetDate, BY_DATE);
//     int index = jumpSearch(transactions, transactionCount, targetDate);
//     // int index = interpolationSearch(transactions, transactionCount, targetDate);
//     // int index = linearSearch(transactions, transactionCount, targetDate); 

//     if (index != -1) {
//         // Expand to find all matching transactions on the same date
//         int left = index - 1;
//         while (left >= 0 && transactions[left].date == targetDate) --left;

//         int right = index + 1;
//         while (right < transactionCount && transactions[right].date == targetDate) ++right;

//         int totalFound = right - (left + 1);

//         for (int i = left + 1; i < right; ++i) {
//             cout << "Customer ID: " << transactions[i].customerID << "\n";
//             cout << "Product: " << transactions[i].product << "\n";
//             cout << "Category: " << transactions[i].category << "\n";
//             cout << "Price: $" << transactions[i].price << "\n";
//             cout << "Date: " << transactions[i].date << "\n";
//             cout << "Payment Method: " << transactions[i].paymentMethod << "\n\n";
//         }

//         cout << "\nTransactions found on " << targetDate << ": " << totalFound << "\n";

//     } else {
//         cout << "No transaction found on that date.\n";
//     }

//     auto end = high_resolution_clock::now();
//     cout << "\nSearch Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

//     delete[] transactions;
//     return 0;
// }





// //Q1 COMPARE
// int main() {
//     // === READ TRANSACTION DATA ===
//     Record* transactions;
//     int transactionCount = readTransactionCSV("transactions_cleaned.csv", transactions);
//     if (transactionCount == 0) return 1;

//     auto start = high_resolution_clock::now();

//     // === SORTING METHOD ===
//     // Uncomment one of the following to sort by different attributes:
//     // bubbleSort(transactions, transactionCount, BY_DATE);
//     // selectionSort(transactions, transactionCount, BY_CATEGORY);
//     // insertionSort(transactions, transactionCount, BY_PRICE);
//     mergeSort(transactions, 0, transactionCount - 1, BY_DATE);  // You can change to BY_CATEGORY or BY_PRICE

//     // === DISPLAY SORTED TRANSACTIONS ===
//     cout << "\n=== SORTED TRANSACTIONS ===\n";
//     displayTransactions(transactions, transactionCount);

//     // === COUNT TOTAL TRANSACTIONS ===
//     cout << "\n=== TRANSACTIONS ===\n";
//     cout << "Total Transactions: " << transactionCount << "\n";

//     // === RUNTIME ===
//     auto end = high_resolution_clock::now();
//     cout << "Sorting Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

//     delete[] transactions;
//     return 0;
// }



// //Q2 FULL
// int main() {
//     Record* transactions;
//     int transactionCount = readTransactionCSV("transactions_cleaned.csv", transactions);
//     if (transactionCount == 0) {
//         cerr << "Failed to load transaction data." << endl;
//         return 1;
//     }

//     auto start = high_resolution_clock::now();

//     processElectronicsCreditCardPercentage(transactions, transactionCount);

//     auto end = high_resolution_clock::now();
//     cout << "\nExecution Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

//     delete[] transactions;
//     return 0;
// }



// Q3
int main() {
    Record* transactions;
    int transactionCount = readTransactionCSV("transactions_cleaned.csv", transactions);
    if (transactionCount == 0) return 1;

    Review* reviews;
    int reviewCount = readReviewCSV("reviews_cleaned.csv", reviews);
    if (reviewCount == 0) return 1;

    // Count before filtering
    cout << "Total Reviews (Raw): " << reviewCount << endl;

    // Filter reviews based on valid transactions
    reviewCount = filterReviews(reviews, reviewCount, transactions, transactionCount);

    // Count after filtering
    cout << "Total Reviews (Filtered): " << reviewCount << endl;

    // Analyze 1-star reviews
    analyzeOneStarReviews(reviews, reviewCount);

    delete[] transactions;
    delete[] reviews;

    return 0;
}

