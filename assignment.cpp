#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <memory>
#include <map>


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

void filterReviews(ReviewNode** reviewHeadRef, TransactionNode* transactionHead, bool debug = false) {
    unordered_map<string, int> transactionCounts;

    // Step 1: Count number of transactions per customer
    while (transactionHead) {
        transactionCounts[transactionHead->data.customerID]++;
        transactionHead = transactionHead->next;
    }

    unordered_map<string, int> reviewCounts;

    // Step 2: Use a dummy node for easier head deletion
    ReviewNode dummy;
    dummy.link = *reviewHeadRef;
    ReviewNode* prev = &dummy;
    ReviewNode* current = dummy.link;

    while (current) {
        string cid = current->customer_id;
        int allowed = transactionCounts.count(cid) ? transactionCounts[cid] : 0;

        if (reviewCounts[cid] < allowed) {
            // Keep this review
            reviewCounts[cid]++;
            prev = current;
            current = current->link;
        } else {
            // Delete the review
            if (debug) {
                cout << "Deleting extra review by customer: " << cid << endl;
            }

            ReviewNode* toDelete = current;
            prev->link = current->link;
            current = current->link;
            delete toDelete;
        }
    }

    // Update head reference after filtering
    *reviewHeadRef = dummy.link;
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

// Clean a word (remove punctuation, lowercase)
string cleanWord(const string& word) {
    string cleaned;
    for (char c : word) {
        if (isalnum(c)) cleaned += tolower(c);
    }
    return cleaned;
}

// Display word frequencies in 1-star reviews sorted in descending order
void displayWordFrequenciesInOneStarReviews(ReviewNode* head) {
    unordered_map<string, int> wordFreq;

    while (head) {
        if (head->rating == 1) {
            stringstream ss(head->review);
            string word;
            while (ss >> word) {
                word = cleanWord(word);
                if (!word.empty()) wordFreq[word]++;
            }
        }
        head = head->link;
    }

    if (wordFreq.empty()) {
        cout << "No 1-star reviews found." << endl;
    } else {
        // Create a multimap to sort words by frequency in descending order
        multimap<int, string, greater<int>> sortedWords;

        // Insert words into the multimap with frequency as the key
        for (const auto& [word, count] : wordFreq) {
            sortedWords.insert({count, word});
        }

        cout << "\nWord frequencies in 1-star reviews (sorted by frequency):" << endl;
        for (const auto& [count, word] : sortedWords) {
            cout << word << ": " << count << endl;
        }
    }
}







// // Q1 Main
// int main() {
//     TransactionNode* transactionHead = readTransactionCSV("transactions_cleaned.csv");
//     if (!transactionHead) return 1;

//     auto start = high_resolution_clock::now();

//     // === SORTING METHOD ===
//     // Uncomment one of the following lines to sort transactions
//     transactionHead = bubbleSort(transactionHead);
//     // transactionHead = selectionSort(transactionHead);
//     // transactionHead = insertionSort(transactionHead);
//     // transactionHead = mergeSort(transactionHead);

//     // === DISPLAY SORTED TRANSACTIONS ===
//     cout << "\n=== SORTED TRANSACTIONS ===\n";
//     displayTransactions(transactionHead);

//     // === COUNT TOTAL TRANSACTIONS ===
//     cout << "\n=== TRANSACTIONS ===\n";
//     int totalTransactions = 0;
//     TransactionNode* current = transactionHead;
//     while (current) {
//         totalTransactions++;
//         current = current->next;
//     }
//     cout << "Total Transactions: " << totalTransactions << "\n";

//     // === RUNTIME ===
//     auto end = high_resolution_clock::now();
//     cout << "Sorting Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

//     return 0;
// }







// // Q3 Main
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

//     ReviewNode* reviewHead = nullptr;
//     char line[512];

//     fgets(line, sizeof(line), file); // skip header

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
//     filterReviews(&reviewHead, transactionHead, true);

//     // Display reviews
//     cout << "\n=== Filtered Reviews ===\n" << endl;
//     displayReviews(reviewHead);
//     cout << "Total Valid Reviews: " << countReviews(reviewHead) << endl;

//     // Display word frequencies in 1-star reviews sorted by frequency
//     displayWordFrequenciesInOneStarReviews(reviewHead);

//     return 0;
// }

