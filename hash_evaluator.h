#ifndef HASH_EVALUATOR_H
#define HASH_EVALUATOR_H

#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <iomanip> // for formatted output

using namespace std;

double calculateBitDifference(const string& hash1, const string& hash2) {
    int diffCount = 0;
    for (size_t i = 0; i < hash1.size(); ++i) {
        // XOR the two characters and count the differing bits
        diffCount += bitset<8>(hash1[i] ^ hash2[i]).count();
    }
    // Return the percentage of differing bits
    return (static_cast<double>(diffCount) / (hash1.size() * 8)) * 100;
}

double calculateHexDifference(const string& hash1, const string& hash2) {
    int diffCount = 0;
    for (size_t i = 0; i < hash1.size(); ++i) {
        if (hash1[i] != hash2[i]) {
            diffCount++;
        }
    }
    // Return the percentage of differing hex characters
    return (static_cast<double>(diffCount) / hash1.size()) * 100;
}

void processInputToHashes(const string& input, vector<string>& hashes) {
    string modifiedInput = input + to_string(input.length());
    string hashResult = bitset<64>(hash<string>{}(modifiedInput)).to_string();
    hashes.push_back(hashResult);  // Store the hash in the vector
}

void evaluateFileHashes(const string& fileName) {
    vector<string> hashes;
    string line;
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    double totalBitDifference = 0;
    double totalHexDifference = 0;
    double minBitDifference = 100, maxBitDifference = 0;
    double minHexDifference = 100, maxHexDifference = 0;
    int pairCount = 0;

    string prevLine = "";  // Store the previous line's hash

    // Process each line and generate hashes
    int lineNumber = 0;
    while (getline(inputFile, line)) {
        vector<string> tempHashes;
        processInputToHashes(line, tempHashes);
        string currentHash = tempHashes.back();
        hashes.push_back(currentHash);

        // If this is the second line in the pair, compare it with the previous line
        if (lineNumber % 2 == 1) {
            string prevHash = hashes[lineNumber - 1];  // Get the previous line's hash

            // Calculate bit-level and hex-level differences
            double bitDiff = calculateBitDifference(prevHash, currentHash);
            double hexDiff = calculateHexDifference(prevHash, currentHash);

            // Update the minimum and maximum differences
            minBitDifference = min(minBitDifference, bitDiff);
            maxBitDifference = max(maxBitDifference, bitDiff);
            minHexDifference = min(minHexDifference, hexDiff);
            maxHexDifference = max(maxHexDifference, hexDiff);

            // Accumulate the total differences
            totalBitDifference += bitDiff;
            totalHexDifference += hexDiff;

            pairCount++;  // Increment the pair count
        }

        lineNumber++;
    }
    inputFile.close();

    // Compute the average differences
    double avgBitDifference = totalBitDifference / pairCount;
    double avgHexDifference = totalHexDifference / pairCount;

    // Output the summary of minimum, maximum, and average differences
    cout << "\nSummary of Differences between Consecutive Pairs:" << endl;
    cout << "Number of pairs: " << pairCount << endl;
    cout << "Min Hex Difference: " << minHexDifference << "%" << endl;
    cout << "Max Hex Difference: " << maxHexDifference << "%" << endl;
    cout << "Avg Hex Difference: " << avgHexDifference << "%" << endl;
    cout << "Min Bit Difference: " << minBitDifference << "%" << endl;
    cout << "Max Bit Difference: " << maxBitDifference << "%" << endl;
    cout << "Avg Bit Difference: " << avgBitDifference << "%" << endl;
}

#endif // HASH_EVALUATOR_H
