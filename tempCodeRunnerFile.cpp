
//Q2 FULL
int main() {
    Record* transactions;
    int transactionCount = readTransactionCSV("transactions_cleaned.csv", transactions);
    if (transactionCount == 0) {
        cerr << "Failed to load transaction data." << endl;
        return 1;
    }

    auto start = high_resolution_clock::now();

    processElectronicsCreditCardPercentage(transactions, transactionCount);

    auto end = high_resolution_clock::now();
    cout << "\nExecution Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

    delete[] transactions;
    return 0;
}