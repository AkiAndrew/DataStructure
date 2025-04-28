        // Insert words into the multimap with frequency as the key
        for (const auto& pair : wordFreq) {
            sortedWords.insert({pair.second, pair.first});
        }

        cout << "\nWord frequencies in 1-star reviews (sorted by frequency):" << endl;
        for (const auto& pair : sortedWords) {
            cout << pair.second << ": " << pair.first << endl;
        } 