#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

// Struct to represent a transaction record
struct Record {
    string customerID;
    string product;
    string category;
    double price;
    string date;
    string paymentMethod;

    // Convert MM/DD/YYYY to YYYYMMDD as integer for sorting
    int dateToInt() const {
        int month, day, year;
        sscanf(date.c_str(), "%d/%d/%d", &day, &month, &year);
        return year * 10000 + month * 100 + day;
    }
};

// Node structure for Linked List
struct Node {
    Record data;
    Node* next;

    Node(Record record) : data(record), next(nullptr) {}
};

// Function to create a new node
Node* createNode(const Record& record) {
    return new Node(record);
}

// Function to append a new node to the linked list
void appendNode(Node*& head, Node* newNode) {
    if (!head) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to read CSV file into linked list
Node* readCSV(const string& filename) {
    ifstream file(filename);
    string line;
    Node* head = nullptr;

    if (!file.is_open()) {
        cerr << "Error: Could not open the file." << endl;
        return nullptr;
    }

    // Skip the header
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

        double price = stod(priceStr);

        Record record = {customerID, product, category, price, date, paymentMethod};
        Node* newNode = createNode(record);
        appendNode(head, newNode);
    }

    file.close();
    return head;
}

// Bubble Sort based on date (for linked list)
void bubbleSortByDate(Node* head) {
    if (!head) return;

    bool swapped;
    do {
        swapped = false;
        Node* current = head;
        while (current && current->next) {
            if (current->data.dateToInt() > current->next->data.dateToInt()) {
                swap(current->data, current->next->data);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

// Selection Sort based on date (for linked list)
void selectionSortByDate(Node* head) {
    if (!head) return;

    Node* current = head;
    while (current) {
        Node* minNode = current;
        Node* nextNode = current->next;
        while (nextNode) {
            if (nextNode->data.dateToInt() < minNode->data.dateToInt()) {
                minNode = nextNode;
            }
            nextNode = nextNode->next;
        }
        if (minNode != current) {
            swap(current->data, minNode->data);
        }
        current = current->next;
    }
}

// Insertion Sort based on date (for linked list)
void insertionSortByDate(Node* head) {
    if (!head) return;

    Node* sorted = nullptr;
    Node* current = head;
    while (current) {
        Node* next = current->next;
        if (!sorted || sorted->data.dateToInt() >= current->data.dateToInt()) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next && temp->next->data.dateToInt() < current->data.dateToInt()) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

// Merge Sort helper functions (for linked list)
Node* merge(Node* left, Node* right) {
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

Node* mergeSort(Node* head) {
    if (!head || !head->next) return head;

    Node* middle = head;
    Node* fast = head->next;

    while (fast && fast->next) {
        middle = middle->next;
        fast = fast->next->next;
    }

    Node* left = head;
    Node* right = middle->next;
    middle->next = nullptr;

    left = mergeSort(left);
    right = mergeSort(right);

    return merge(left, right);
}

// Function to measure and output sorting runtime
void countSortRuntime(Node* head, const string& sortType) {
    auto start = high_resolution_clock::now();

    // Sort based on selected sort type
    if (sortType == "bubble") {
        bubbleSortByDate(head);
    } else if (sortType == "selection") {
        selectionSortByDate(head);
    } else if (sortType == "insertion") {
        insertionSortByDate(head);
    } else if (sortType == "merge") {
        head = mergeSort(head);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Sorting completed in " << duration.count() << " ms using " << sortType << " sort." << endl;
}

// Function to display the linked list
void displayList(Node* head) {
    Node* current = head;
    while (current) {
        cout << current->data.customerID << "," << current->data.product << ","
             << current->data.category << "," << current->data.price << ","
             << current->data.date << "," << current->data.paymentMethod << endl;
        current = current->next;
    }
}

int main() {
    string filename = "transactions_cleaned.csv"; // Provide your file path here
    Node* head = readCSV(filename);

    if (!head) {
        cout << "No data found or file couldn't be read." << endl;
        return 1;
    }



    // Measure and sort using different algorithms
    countSortRuntime(head, "bubble");
    // countSortRuntime(head, "selection");
    // countSortRuntime(head, "insertion");
    // countSortRuntime(head, "merge");

    // Display the sorted list
    // cout << "\n=== Sorted List ===" << endl;
    // displayList(head);

    return 0;
}












// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <cctype>
// #include <set>
// #include <string.h>

// using namespace std;

// // Function to clean and lowercase a word
// string cleanWord(const string& word) {
//     string cleaned;
//     for (char c : word) {
//         if (isalpha(c) || isdigit(c)) {
//             cleaned += tolower(c);
//         }
//     }
//     return cleaned;
// }

// // Maximum number of unique words expected
// const int MAX_WORDS = 10000;

// int main() {
//     ifstream file("C:\\Users\\andre\\Downloads\\reviews_cleaned.csv");
//     if (!file.is_open()) {
//         cerr << "Failed to open file.\n";
//         return 1;
//     }

//     string line;
//     getline(file, line); // skip header

//     string words[MAX_WORDS];
//     int counts[MAX_WORDS];
//     int wordCount = 0;

//     set<string> stopwords = {"the", "and", "was", "is", "a", "to", "of", "in", "it", "for", "on", "this", "that"};

//     while (getline(file, line)) {
//         stringstream ss(line);
//         string productId, customerId, ratingStr, reviewText;

//         getline(ss, productId, ',');
//         getline(ss, customerId, ',');
//         getline(ss, ratingStr, ',');
//         getline(ss, reviewText); // rest is review

//         int rating = stoi(ratingStr);
//         if (rating == 1) {
//             stringstream reviewStream(reviewText);
//             string word;
//             while (reviewStream >> word) {
//                 string cleaned = cleanWord(word);
//                 if (cleaned.empty() || stopwords.find(cleaned) != stopwords.end()) {
//                     continue;
//                 }

//                 // Check if the word already exists
//                 int i;
//                 for (i = 0; i < wordCount; ++i) {
//                     if (words[i] == cleaned) {
//                         counts[i]++;
//                         break;
//                     }
//                 }
//                 // If not found, add to array
//                 if (i == wordCount && wordCount < MAX_WORDS) {
//                     words[wordCount] = cleaned;
//                     counts[wordCount] = 1;
//                     wordCount++;
//                 }
//             }
//         }
//     }

//     file.close();

//     // Sort words by frequency (simple bubble sort for illustration)
//     for (int i = 0; i < wordCount - 1; ++i) {
//         for (int j = i + 1; j < wordCount; ++j) {
//             if (counts[j] > counts[i]) {
//                 swap(counts[i], counts[j]);
//                 swap(words[i], words[j]);
//             }
//         }
//     }

//     cout << "Most frequent words in 1-star reviews:\n";
//     for (int i = 0; i < wordCount; ++i) {
//         cout << words[i] << ": " << counts[i] << endl;
//     }

//     return 0;
// }





