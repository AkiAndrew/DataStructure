#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <memory>

using namespace std;
using namespace chrono;

// ---------------- Transaction Record & Linked List ----------------

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

TransactionNode* createTransactionNode(const Record& record) {
    return new TransactionNode(record);
}

void appendTransactionNode(TransactionNode*& head, TransactionNode* newNode) {
    if (!head) head = newNode;
    else {
        TransactionNode* current = head;
        while (current->next) current = current->next;
        current->next = newNode;
    }
}

TransactionNode* readTransactionCSV(const string& filename) {
    ifstream file(filename);
    string line;
    TransactionNode* head = nullptr;

    if (!file.is_open()) {
        cerr << "Error: Could not open transaction file." << endl;
        return nullptr;
    }

    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;
        getline(ss, customerID, ',');
        getline(ss, product, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, date, ',');
        getline(ss, paymentMethod, ',');

        double price = stod(priceStr);

        Record record = {customerID, product, category, price, date, paymentMethod};
        TransactionNode* newNode = createTransactionNode(record);
        appendTransactionNode(head, newNode);
    }

    file.close();
    return head;
}

// ---------------- Sorting Algorithms ----------------

TransactionNode* bubbleSort(TransactionNode* head) {
    if (!head) return nullptr;
    bool swapped;
    do {
        swapped = false;
        TransactionNode* current = head;
        while (current->next) {
            if (current->data.dateToInt() > current->next->data.dateToInt()) {
                swap(current->data, current->next->data);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
    return head;
}

TransactionNode* selectionSort(TransactionNode* head) {
    for (TransactionNode* current = head; current; current = current->next) {
        TransactionNode* minNode = current;
        for (TransactionNode* next = current->next; next; next = next->next) {
            if (next->data.dateToInt() < minNode->data.dateToInt())
                minNode = next;
        }
        if (minNode != current)
            swap(current->data, minNode->data);
    }
    return head;
}

TransactionNode* insertionSort(TransactionNode*& head) {
    TransactionNode* sorted = nullptr;
    TransactionNode* current = head;
    while (current) {
        TransactionNode* next = current->next;
        if (!sorted || sorted->data.dateToInt() >= current->data.dateToInt()) {
            current->next = sorted;
            sorted = current;
        } else {
            TransactionNode* temp = sorted;
            while (temp->next && temp->next->data.dateToInt() < current->data.dateToInt())
                temp = temp->next;
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
    return head;
}

TransactionNode* merge(TransactionNode* left, TransactionNode* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->data.dateToInt() < right->data.dateToInt()) {
        left->next = merge(left->next, right);
        return left;
    } else {
        right->next = merge(left, right->next);
        return right;
    }
}

TransactionNode* mergeSort(TransactionNode* head) {
    if (!head || !head->next) return head;

    TransactionNode* slow = head;
    TransactionNode* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    TransactionNode* mid = slow->next;
    slow->next = nullptr;

    TransactionNode* left = mergeSort(head);
    TransactionNode* right = mergeSort(mid);

    return merge(left, right);
}

// ---------------- Display Transactions ----------------

void displayTransactions(TransactionNode* head) {
    while (head) {
        cout << "Customer ID: " << head->data.customerID << "\n";
        cout << "Product: " << head->data.product << "\n";
        cout << "Category: " << head->data.category << "\n";
        cout << "Price: $" << head->data.price << "\n";
        cout << "Date: " << head->data.date << "\n";
        cout << "Payment Method: " << head->data.paymentMethod << "\n\n";
        head = head->next;
    }
}

// ---------------- Review List ----------------

struct ReviewNode {
    string product_id;
    string customer_id;
    int rating;
    string review;
    ReviewNode* link;
};

ReviewNode* createReviewNode(const string& product_id, const string& customer_id, int rating, const string& review) {
    auto node = new ReviewNode{product_id, customer_id, rating, review, nullptr};
    return node;
}

void appendReviewNode(ReviewNode** head, ReviewNode* node) {
    if (!*head) *head = node;
    else {
        ReviewNode* current = *head;
        while (current->link) current = current->link;
        current->link = node;
    }
}

void displayReviews(ReviewNode* head) {
    while (head) {
        cout << "Product ID: " << head->product_id << "\n";
        cout << "Customer ID: " << head->customer_id << "\n";
        cout << "Rating: " << head->rating << "\n";
        cout << "Review: " << head->review << "\n\n";
        head = head->link;
    }
}

int countReviews(ReviewNode* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->link;
    }
    return count;
}

// ---------------- Filter Reviews ----------------

void filterReviews(ReviewNode** reviewHeadRef, TransactionNode* transactionHead) {
    unordered_map<string, int> transactionCounts;
    while (transactionHead) {
        transactionCounts[transactionHead->data.customerID]++;
        transactionHead = transactionHead->next;
    }

    unordered_map<string, int> reviewCounts;
    unordered_map<string, int> skipCounts;

    ReviewNode* current = *reviewHeadRef;
    ReviewNode* prev = nullptr;

    while (current) {
        string cid = current->customer_id;
        reviewCounts[cid]++;

        if (reviewCounts[cid] > transactionCounts[cid]) {
            skipCounts[cid]++;
            if (skipCounts[cid] % 2 == 1) {
                ReviewNode* toDelete = current;
                if (!prev) {
                    *reviewHeadRef = current->link;
                    current = *reviewHeadRef;
                } else {
                    prev->link = current->link;
                    current = prev->link;
                }
                delete toDelete;
                continue;
            }
        }

        prev = current;
        current = current->link;
    }
}

// ---------------- Save Reviews to CSV ----------------

void saveReviewsToCSV(ReviewNode* head, const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open output file." << endl;
        return;
    }

    outFile << "product_id,customer_id,rating,review\n";

    ReviewNode* current = head;
    while (current != nullptr) {
        outFile << current->product_id << ',' << current->customer_id << ',' 
                << current->rating << ',' << '"' << current->review << '"' << '\n';
        current = current->link;
    }

    outFile.close();
    cout << "\nFiltered reviews saved to '" << filename << "'" << endl;
}


// int main() {
//     // Read transactions
//     TransactionNode* transactionHead = readTransactionCSV("transactions_cleaned.csv");
//     if (!transactionHead) {
//         cout << "Failed to read transaction file." << endl;
//         return 1;
//     }

//     // Read reviews
//     FILE* file = fopen("reviews_cleaned.csv", "r");
//     if (!file) {
//         cout << "Failed to open review file." << endl;
//         return 1;
//     }

//     ReviewNode* reviewHead = NULL;
//     char line[512];

//     fgets(line, sizeof(line), file); // Skip header

//     while (fgets(line, sizeof(line), file)) {
//         char* product_id = strtok(line, ",");
//         char* customer_id = strtok(NULL, ",");
//         char* rating_str = strtok(NULL, ",");
//         char* review = strtok(NULL, "\n");

//         if (product_id && customer_id && rating_str && review) {
//             int rating = atof(rating_str);
//             ReviewNode* new_node = createReviewNode(product_id, customer_id, rating, review);
//             appendReviewNode(&reviewHead, new_node);
//         }
//     }

//     fclose(file);

//     // Filter reviews
//     filterReviews(&reviewHead, transactionHead);

//     // Display & Save
//     cout << "\n=== Filtered Reviews ===\n" << endl;
//     displayReviews(reviewHead);
//     cout << "Total Valid Reviews: " << countReviews(reviewHead) << endl;

//     saveReviewsToCSV(reviewHead, "filtered_reviews.csv");

//     return 0;
// }


// ---------------- Main ----------------

int main() {
    TransactionNode* transactionHead = readTransactionCSV("transactions_cleaned.csv");
    if (!transactionHead) return 1;

    auto start = high_resolution_clock::now();

    // === SORTING METHOD ===
    // Uncomment one of the following lines to sort transactions
    transactionHead = bubbleSort(transactionHead);
    // transactionHead = selectionSort(transactionHead);
    // transactionHead = insertionSort(transactionHead);
    // transactionHead = mergeSort(transactionHead);

    // === DISPLAY SORTED TRANSACTIONS ===
    cout << "\n=== SORTED TRANSACTIONS ===\n";
    displayTransactions(transactionHead);

    // === COUNT TOTAL TRANSACTIONS ===
    cout << "\n=== TRANSACTIONS ===\n";
    int totalTransactions = 0;
    TransactionNode* current = transactionHead;
    while (current) {
        totalTransactions++;
        current = current->next;
    }
    cout << "Total Transactions: " << totalTransactions << "\n";

    // === RUNTIME ===
    auto end = high_resolution_clock::now();
    cout << "Sorting Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    return 0;
}




















// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <unordered_set>
// #include <unordered_map>
// #include <cstring>
// #include <cstdlib>

// using namespace std;

// // ------------------------------------------
// // Transaction Node and Logic
// // ------------------------------------------

// struct Record {
//     string customerID;
//     string product;
//     string category;
//     double price;
//     string date;
//     string paymentMethod;

//     int dateToInt() const {
//         int month, day, year;
//         sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);
//         return year * 10000 + month * 100 + day;
//     }
// };

// struct TransactionNode {
//     Record data;
//     TransactionNode* next;
//     TransactionNode(Record record) : data(record), next(nullptr) {}
// };

// TransactionNode* createTransactionNode(const Record& record) {
//     return new TransactionNode(record);
// }

// void appendTransactionNode(TransactionNode*& head, TransactionNode* newNode) {
//     if (!head) {
//         head = newNode;
//     } else {
//         TransactionNode* current = head;
//         while (current->next) {
//             current = current->next;
//         }
//         current->next = newNode;
//     }
// }

// TransactionNode* readTransactionCSV(const string& filename) {
//     ifstream file(filename);
//     string line;
//     TransactionNode* head = nullptr;

//     if (!file.is_open()) {
//         cerr << "Error: Could not open transaction file." << endl;
//         return nullptr;
//     }

//     getline(file, line); // Skip header

//     while (getline(file, line)) {
//         stringstream ss(line);
//         string customerID, product, category, priceStr, date, paymentMethod;
//         getline(ss, customerID, ',');
//         getline(ss, product, ',');
//         getline(ss, category, ',');
//         getline(ss, priceStr, ',');
//         getline(ss, date, ',');
//         getline(ss, paymentMethod, ',');

//         double price = stod(priceStr);

//         Record record = {customerID, product, category, price, date, paymentMethod};
//         TransactionNode* newNode = createTransactionNode(record);
//         appendTransactionNode(head, newNode);
//     }

//     file.close();
//     return head;
// }

// // ------------------------------------------
// // Review Node and Logic (C-style)
// // ------------------------------------------

// struct ReviewNode {
//     char product_id[50];
//     char customer_id[50];
//     int rating;
//     char review[200];
//     ReviewNode* link;
// };

// ReviewNode* createReviewNode(char product_id[], char customer_id[], int rating, char review[]) {
//     ReviewNode* new_node = (ReviewNode*)malloc(sizeof(ReviewNode));
//     strcpy(new_node->product_id, product_id);
//     strcpy(new_node->customer_id, customer_id);
//     new_node->rating = rating;
//     strcpy(new_node->review, review);
//     new_node->link = NULL;
//     return new_node;
// }

// void appendReviewNode(ReviewNode** head_ref, ReviewNode* new_node) {
//     if (*head_ref == NULL) {
//         *head_ref = new_node;
//         return;
//     }
//     ReviewNode* current = *head_ref;
//     while (current->link != NULL) {
//         current = current->link;
//     }
//     current->link = new_node;
// }

// void displayReviews(ReviewNode* head) {
//     ReviewNode* current = head;
//     while (current != NULL) {
//         printf("Product ID: %s\n", current->product_id);
//         printf("Customer ID: %s\n", current->customer_id);
//         printf("Rating: %d\n", current->rating);
//         printf("Review: %s\n\n", current->review);
//         current = current->link;
//     }
// }

// int countReviews(ReviewNode* head) {
//     int count = 0;
//     ReviewNode* current = head;
//     while (current != NULL) {
//         count++;
//         current = current->link;
//     }
//     return count;
// }

// // ------------------------------------------
// // Filter and Save Logic
// // ------------------------------------------

// void filterReviews(ReviewNode** reviewHeadRef, TransactionNode* transactionHead) {
//     unordered_map<string, int> transactionCounts;

//     // Count transactions per customer
//     TransactionNode* tx = transactionHead;
//     while (tx != nullptr) {
//         transactionCounts[tx->data.customerID]++;
//         tx = tx->next;
//     }

//     unordered_map<string, int> reviewCounts;
//     unordered_map<string, int> skipCounts;

//     ReviewNode* current = *reviewHeadRef;
//     ReviewNode* prev = nullptr;

//     while (current != nullptr) {
//         string customerID = current->customer_id;
//         reviewCounts[customerID]++;

//         int allowedReviews = transactionCounts[customerID];

//         if (reviewCounts[customerID] > allowedReviews) {
//             // Delete every alternate excess review
//             skipCounts[customerID]++;
//             if (skipCounts[customerID] % 2 == 1) {
//                 ReviewNode* toDelete = current;
//                 if (prev == nullptr) {
//                     *reviewHeadRef = current->link;
//                     current = *reviewHeadRef;
//                 } else {
//                     prev->link = current->link;
//                     current = prev->link;
//                 }
//                 free(toDelete);
//                 continue;
//             }
//         }

//         prev = current;
//         current = current->link;
//     }
// }

// void saveReviewsToCSV(ReviewNode* head, const string& filename) {
//     ofstream outFile(filename);
//     if (!outFile.is_open()) {
//         cerr << "Error: Could not open output file." << endl;
//         return;
//     }

//     outFile << "product_id,customer_id,rating,review\n";

//     ReviewNode* current = head;
//     while (current != nullptr) {
//         outFile << current->product_id << ','
//                 << current->customer_id << ','
//                 << current->rating << ','
//                 << '"' << current->review << '"' << '\n';
//         current = current->link;
//     }

//     outFile.close();
//     cout << "\nFiltered reviews saved to '" << filename << "'" << endl;
// }

// // ------------------------------------------
// // Main
// // ------------------------------------------

// int main() {
//     // Read transactions
//     TransactionNode* transactionHead = readTransactionCSV("transactions_cleaned.csv");
//     if (!transactionHead) {
//         cout << "Failed to read transaction file." << endl;
//         return 1;
//     }

//     // Read reviews
//     FILE* file = fopen("reviews_cleaned.csv", "r");
//     if (!file) {
//         cout << "Failed to open review file." << endl;
//         return 1;
//     }

//     ReviewNode* reviewHead = NULL;
//     char line[512];

//     fgets(line, sizeof(line), file); // Skip header

//     while (fgets(line, sizeof(line), file)) {
//         char* product_id = strtok(line, ",");
//         char* customer_id = strtok(NULL, ",");
//         char* rating_str = strtok(NULL, ",");
//         char* review = strtok(NULL, "\n");

//         if (product_id && customer_id && rating_str && review) {
//             int rating = atof(rating_str);
//             ReviewNode* new_node = createReviewNode(product_id, customer_id, rating, review);
//             appendReviewNode(&reviewHead, new_node);
//         }
//     }

//     fclose(file);

//     // Filter reviews
//     filterReviews(&reviewHead, transactionHead);

//     // Display & Save
//     cout << "\n=== Filtered Reviews ===\n" << endl;
//     displayReviews(reviewHead);
//     cout << "Total Valid Reviews: " << countReviews(reviewHead) << endl;

//     saveReviewsToCSV(reviewHead, "filtered_reviews.csv");

//     return 0;
// }
