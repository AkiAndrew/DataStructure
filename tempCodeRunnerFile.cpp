MPARE
int main() {
    // === READ TRANSACTION DATA ===
    Record* transactions;
    int transactionCount = readTransactionCSV("transactions_cleaned.csv", transactions);
    if (transactionCount == 0) return 1;

    auto start = high_resolution_clock::now();

    // === SORTING METHOD ===
    // Uncomment one of the following to sort by different attributes:
    // bubbleSort(transactions, transactionCount, BY_DATE);
    // selectionSort(transactions, transactionCount, BY_CATEGORY);
    // insertionSort(transactions, transactionCount, BY_PRICE);
    mergeSort(transactions, 0, transactionCount - 1, BY_DATE);  // You can change to BY_CATEGORY or BY_PRICE

    // === DISPLAY SORTED TRANSACTIONS ===
    cout << "\n=== SORTED TRANSACTIONS ===\n";
    displayTransactions(transactions, transactionCount);

    // === COUNT TOTAL TRANSACTIONS ===
    cout << "\n=== TRANSACTIONS ===\n";
    cout << "Total Transactions: " << transactionCount << "\n";

    // === RUNTIME ===
    auto end = high_resolution_clock::now();
    cout << "Sorting Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    delete[] transactions;
    return 0;
}