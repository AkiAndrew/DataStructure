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
#include <cmath>
#include <iomanip> 


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


// ---------------- Searching algorithms ----------------
// Linear Search
void linearSearchByDate(TransactionNode* head, const string& targetDate) {
    bool found = false;
    while (head) {
        if (head->data.date == targetDate) {
            cout << "Customer ID: " << head->data.customerID << "\n";
            cout << "Product: " << head->data.product << "\n";
            cout << "Category: " << head->data.category << "\n";
            cout << "Price: $" << head->data.price << "\n";
            cout << "Date: " << head->data.date << "\n";
            cout << "Payment Method: " << head->data.paymentMethod << "\n\n";
            found = true;
        }
        head = head->next;
    }
    if (!found) {
        cout << "No transactions found on the given date.\n";
    }
}

// Binary Search
TransactionNode* findMiddle(TransactionNode* head) {
    if (!head) return nullptr;
    
    TransactionNode* slow = head;
    TransactionNode* fast = head;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

TransactionNode* binarySearch(TransactionNode* head, const string& targetDate) {
    if (!head) return nullptr;  // Base case: if the list is empty

    // Find the middle node
    TransactionNode* middle = findMiddle(head);
    
    // If middle node's date matches the target date, return the node
    if (middle->data.date == targetDate) {
        return middle;
    }
    
    // If the target date is less than the middle date, search the left sublist
    if (targetDate < middle->data.date) {
        return binarySearch(head, targetDate);  // Left half
    }
    
    // If the target date is greater than the middle date, search the right sublist
    else {
        return binarySearch(middle->next, targetDate);  // Right half
    }
}

void binarySearchByDate(TransactionNode* head, const string& targetDate) {
    TransactionNode* current = head;
    bool found = false;

    // Traverse through the linked list and check each transaction
    while (current) {
        if (current->data.date == targetDate) {
            if (!found) {
                cout << "Transactions found on date " << targetDate << ":\n";
                found = true;
            }
            cout << "Customer ID: " << current->data.customerID << "\n";
            cout << "Product: " << current->data.product << "\n";
            cout << "Category: " << current->data.category << "\n";
            cout << "Price: $" << current->data.price << "\n";
            cout << "Date: " << current->data.date << "\n";
            cout << "Payment Method: " << current->data.paymentMethod << "\n\n";
        }
        current = current->next;
    }

    if (!found) {
        cout << "No transactions found for the given date.\n";
    }
}

// Jump Search
int dateToInt(const string& date) {
    // Date format is DD/MM/YYYY
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    return year * 10000 + month * 100 + day; // YYYYMMDD format
}

void jumpSearchByDate(TransactionNode* head, const string& targetDate) {
    // if (!head) {
    //     cout << "No transactions found for the given date.\n";
    //     return;
    // }

    // Count total number of nodes
    int n = 0;
    TransactionNode* temp = head;
    while (temp) {
        n++;
        temp = temp->next;
    }

    int jumpSize = sqrt(n);

    TransactionNode* prev = head;
    TransactionNode* curr = head;
    int steps = 0;

    // Jumping phase
    while (curr->next && dateToInt(curr->data.date) < dateToInt(targetDate)) {
        prev = curr;
        steps = 0;
        while (steps < jumpSize && curr->next) {
            curr = curr->next;
            steps++;
        }
    }

    // Linear search phase
    bool found = false;
    temp = prev;
    while (temp != curr->next) { // careful: must check up to curr
        if (temp->data.date == targetDate) {
            if (!found) {
                cout << "Transactions found on date " << targetDate << ":\n";
                found = true;
            }
            cout << "Customer ID: " << temp->data.customerID << "\n";
            cout << "Product: " << temp->data.product << "\n";
            cout << "Category: " << temp->data.category << "\n";
            cout << "Price: $" << temp->data.price << "\n";
            cout << "Date: " << temp->data.date << "\n";
            cout << "Payment Method: " << temp->data.paymentMethod << "\n\n";
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "No transactions found for the given date.\n";
    }
}

// Interpolation search 
void interpolationSearchByDate(TransactionNode* head, const string& targetDate) {
    if (!head) {
        cout << "No transactions found.\n";
        return;
    }

    bool found = false;
    cout << "Transactions found on date " << targetDate << ":\n";

    // Count total number of nodes and find min/max dates
    int n = 0;
    TransactionNode* temp = head;
    string minDate = temp->data.date;
    string maxDate = temp->data.date;
    
    while (temp) {
        n++;
        if (dateToInt(temp->data.date) < dateToInt(minDate)) {
            minDate = temp->data.date;
        }
        if (dateToInt(temp->data.date) > dateToInt(maxDate)) {
            maxDate = temp->data.date;
        }
        temp = temp->next;
    }

    // Check if target date is outside the range
    if (dateToInt(targetDate) < dateToInt(minDate) || dateToInt(targetDate) > dateToInt(maxDate)) {
        cout << "No transactions found on date " << targetDate << ".\n";
        return;
    }

    // Apply interpolation search to find an entry with the target date
    int low = 0;
    int high = n - 1;
    int pos;
    
    int targetDateInt = dateToInt(targetDate);
    int minDateInt = dateToInt(minDate);
    int maxDateInt = dateToInt(maxDate);
    
    // Guard against division by zero
    if (minDateInt == maxDateInt) {
        pos = 0;  // If all dates are the same, start from the beginning
    } else {
        pos = low + ((targetDateInt - minDateInt) * (high - low)) / (maxDateInt - minDateInt);
    }
    
    if (pos < 0) pos = 0;
    if (pos >= n) pos = n - 1;
    
    // Find a transaction with the target date
    TransactionNode* current = head;
    for (int i = 0; i < pos; i++) {
        current = current->next;
    }
    
    // If we didn't hit the target date directly, search linearly from the position
    while (current && dateToInt(current->data.date) < targetDateInt) {
        current = current->next;
    }
    
    // Now, print all transactions with the target date
    while (current && current->data.date == targetDate) {
        cout << "Customer ID: " << current->data.customerID << "\n";
        cout << "Product: " << current->data.product << "\n";
        cout << "Category: " << current->data.category << "\n";
        cout << "Price: $" << current->data.price << "\n";
        cout << "Date: " << current->data.date << "\n";
        cout << "Payment Method: " << current->data.paymentMethod << "\n\n";
        
        found = true;
        current = current->next;
    }
    
    // Start from the beginning to check for earlier occurrences
    // This is needed because interpolation might position us after some matches
    current = head;
    while (current && dateToInt(current->data.date) <= targetDateInt) {
        if (current->data.date == targetDate) {
            // Check if we already printed this transaction
            if (!found) {
                cout << "Customer ID: " << current->data.customerID << "\n";
                cout << "Product: " << current->data.product << "\n";
                cout << "Category: " << current->data.category << "\n";
                cout << "Price: $" << current->data.price << "\n";
                cout << "Date: " << current->data.date << "\n";
                cout << "Payment Method: " << current->data.paymentMethod << "\n\n";
                found = true;
            }
        }
        current = current->next;
    }
    
    if (!found) {
        cout << "No transactions found on date " << targetDate << ".\n";
    }
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

ReviewNode* mergeByReviewLength(ReviewNode* a, ReviewNode* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->review.length() > b->review.length()) {
        a->link = mergeByReviewLength(a->link, b);
        return a;
    } else {
        b->link = mergeByReviewLength(a, b->link);
        return b;
    }
}

ReviewNode* mergeSortByReviewLength(ReviewNode* head) {
    if (!head || !head->link) return head;

    ReviewNode* slow = head;
    ReviewNode* fast = head->link;

    while (fast && fast->link) {
        slow = slow->link;
        fast = fast->link->link;
    }

    ReviewNode* mid = slow->link;
    slow->link = nullptr;

    ReviewNode* left = mergeSortByReviewLength(head);
    ReviewNode* right = mergeSortByReviewLength(mid);

    return mergeByReviewLength(left, right);
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
        for (const auto& pair : wordFreq) {
            sortedWords.insert({pair.second, pair.first});
        }

        cout << "\nWord frequencies in 1-star reviews (sorted by frequency):" << endl;
        for (const auto& pair : sortedWords) {
            cout << pair.second << ": " << pair.first << endl;
        }
    }
}

// Function to get node at position 'pos' in linked list
TransactionNode* getNodeAtPosition(TransactionNode* head, int pos) {
    TransactionNode* current = head;
    int count = 0;
    
    while (current && count < pos) {
        current = current->next;
        count++;
    }
    
    return current;
}

// Function to get length of linked list
int getListLength(TransactionNode* head) {
    int length = 0;
    TransactionNode* current = head;
    
    while (current) {
        length++;
        current = current->next;
    }
    
    return length;
}


void processElectronicsCreditCardPercentage(TransactionNode* head) {
    if (!head) {
        cout << "No transactions found." << endl;
        return;
    }

    int totalElectronics = 0;
    int creditCardElectronics = 0;
    
    // First, sort the list by category to optimize search
    TransactionNode* sortedHead = mergeSort(head);

    cout << "\n=== ELECTRONICS CATEGORY PAYMENT ANALYSIS ===\n";
   
    // Binary Search
    // Simply count all Electronics transactions with a single pass through the list
    TransactionNode* current = sortedHead;
    while (current) {
        // Make sure to compare case-insensitively or exactly as stored
        if (current->data.category == "Electronics") {
            totalElectronics++;
            if (current->data.paymentMethod == "Credit Card") {
                creditCardElectronics++;
            }
        }
        current = current->next;
    }


    // Calculate percentage
    double percentage = (totalElectronics > 0) ? 
                         (static_cast<double>(creditCardElectronics) / totalElectronics) * 100.0 : 0;
    
    // Display results
    cout << "Total Electronics Transactions: " << totalElectronics << endl;
    cout << "Electronics transactions paid via Credit Card: " << creditCardElectronics << endl;
    cout << "Percentage of Electronics purchases made using Credit Card: " << fixed << setprecision(2) << percentage << "%" << endl;
}





// // Q1 FULL
// int main() {
//     // Read the transaction data from the CSV file
//     TransactionNode* transactionHead = readTransactionCSV("transactions_cleaned.csv");
//     if (!transactionHead) {
//         cerr << "Failed to load transaction data." << endl;
//         return 1;
//     }

//     // === SORT METHOD ===
//     transactionHead = mergeSort(transactionHead);

//     string targetDate;
//     cout << "Enter date to search (format DD/MM/YYYY): ";
//     getline(cin, targetDate);

//     auto start = high_resolution_clock::now();

//     // === SEARCH METHOD ===
//     // jumpSearchByDate(transactionHead, targetDate);
//     // interpolationSearchByDate(transactionHead, targetDate);
//     // linearSearchByDate(transactionHead, targetDate);
//     binarySearchByDate(transactionHead, targetDate);
    
//     auto end = high_resolution_clock::now();
//     cout << "\nSearch Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

//     // Free memory
//     TransactionNode* current = transactionHead;
//     while (current) {
//         TransactionNode* temp = current;
//         current = current->next;
//         delete temp;
//     }

//     return 0;
// }



// // Q1 COMPARE
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







// // Q2 FULL
// int main() {
//     TransactionNode* transactions = readTransactionCSV("transactions_cleaned.csv");
//     if (!transactions) {
//         cerr << "Failed to load transaction data." << endl;
//         return 1;
//     }

//     auto start = high_resolution_clock::now();
    
//     processElectronicsCreditCardPercentage(transactions);
    
//     auto end = high_resolution_clock::now();
//     cout << "\nExecution Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

//     // Free memory
//     TransactionNode* current = transactions;
//     while (current) {
//         TransactionNode* temp = current;
//         current = current->next;
//         delete temp;
//     }

//     return 0;
// }



// Q3 FULL
int main() {
    // Read transaction data
    TransactionNode* transactionHead = readTransactionCSV("transactions_cleaned.csv");
    if (!transactionHead) {
        cout << "Failed to read transaction file." << endl;
        return 1;
    }

    // Read review data
    FILE* file = fopen("reviews_cleaned.csv", "r");
    if (!file) {
        cout << "Failed to open review file." << endl;
        return 1;
    }

    ReviewNode* reviewHead = nullptr;
    char line[512];
    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        char* product_id = strtok(line, ",");
        char* customer_id = strtok(NULL, ",");
        char* rating_str = strtok(NULL, ",");
        char* review = strtok(NULL, "\n");

        if (product_id && customer_id && rating_str && review) {
            int rating = atoi(rating_str);
            ReviewNode* newNode = createReviewNode(product_id, customer_id, rating, review);
            appendReviewNode(&reviewHead, newNode);
        }
    }
    fclose(file);

    // Filter invalid reviews based on transactions
    filterReviews(&reviewHead, transactionHead);

    // Extract 1-star reviews only
    ReviewNode* oneStarHead = nullptr;
    ReviewNode** tail = &oneStarHead;
    for (ReviewNode* curr = reviewHead; curr; curr = curr->link) {
        if (curr->rating == 1) {
            ReviewNode* copy = createReviewNode(curr->product_id, curr->customer_id, curr->rating, curr->review);
            *tail = copy;
            tail = &((*tail)->link);
        }
    }

    // Step 1: Sort 1-star reviews by review length using merge sort
    oneStarHead = mergeSortByReviewLength(oneStarHead);

    // Step 2: Display sorted 1-star reviews (optional)
    cout << "\n=== Sorted 1-Star Reviews ===\n";
    displayReviews(oneStarHead);

    // Step 3: Count the number of 1-star reviews
    int oneStarReviewCount = 0;
    for (ReviewNode* curr = oneStarHead; curr != nullptr; curr = curr->link) {
        oneStarReviewCount++;
    }

    // Step 4: Display the count of 1-star reviews
    cout << "\nTotal number of 1-star reviews: " << oneStarReviewCount << endl;

    // Step 5: Count word frequencies in 1-star reviews
    unordered_map<string, int> wordFreq;

    for (ReviewNode* current = oneStarHead; current != nullptr; current = current->link) {
        stringstream ss(current->review);
        string word;
        while (ss >> word) {
            word = cleanWord(word);  // Clean the word (remove punctuation, lowercase)
            if (!word.empty()) wordFreq[word]++;
        }
    }

    // Step 6: Store word frequencies in a multimap sorted by frequency (descending order)
    multimap<int, string, greater<int>> sortedWords;

    for (const auto& [word, count] : wordFreq) {
        sortedWords.insert({count, word});
    }

    // Step 7: Display all frequent words in 1-star reviews
    if (!sortedWords.empty()) {
        cout << "\nWord Frequencies in 1-Star Reviews (sorted by frequency):\n";
        for (const auto& [count, word] : sortedWords) {
            cout << word << ": " << count << endl;
        }
    } else {
        cout << "\nNo 1-star reviews found." << endl;
    }

    return 0;
}