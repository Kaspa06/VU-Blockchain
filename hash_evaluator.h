#ifndef HASH_EVALUATOR_H
#define HASH_EVALUATOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <iomanip>  // Added for std::setw and std::setfill
#include "SHA256.h" // Ensure SHA256.h is included

using namespace std;

const int HASH_SIZE = 32; 

double calculateBitDifference(const std::array<uint8_t, HASH_SIZE>& hash1, const std::array<uint8_t, HASH_SIZE>& hash2) {
    int differentBits = 0;
    int totalBits = HASH_SIZE * 8;

    for (size_t i = 0; i < HASH_SIZE; ++i) {
        differentBits += __builtin_popcount(hash1[i] ^ hash2[i]);
    }

    return (static_cast<double>(differentBits) / totalBits) * 100;
}

std::string toHexString(const std::array<uint8_t, HASH_SIZE>& hash) {
    std::ostringstream oss;
    for (auto byte : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

double calculateHexDifference(const std::string& hex1, const std::string& hex2) {
    int differentHexDigits = 0;
    int totalHexDigits = hex1.size();

    for (size_t i = 0; i < totalHexDigits; ++i) {
        if (hex1[i] != hex2[i]) {
            differentHexDigits++;
        }
    }

    return (static_cast<double>(differentHexDigits) / totalHexDigits) * 100;
}

void analyzeHashDifferences(const std::vector<std::array<uint8_t, HASH_SIZE>>& hashValues) {
    std::vector<double> bitDifferences;
    std::vector<double> hexDifferences;

    for (size_t i = 0; i < hashValues.size(); i += 2) {  // Process in pairs
        if (i + 1 < hashValues.size()) {  // Ensure there is a next hash to compare
            double bitDiff = calculateBitDifference(hashValues[i], hashValues[i + 1]);
            bitDifferences.push_back(bitDiff);

            std::string hex1 = toHexString(hashValues[i]);
            std::string hex2 = toHexString(hashValues[i + 1]);
            double hexDiff = calculateHexDifference(hex1, hex2);
            hexDifferences.push_back(hexDiff);
        }
    }

    double avgBitDiff = std::accumulate(bitDifferences.begin(), bitDifferences.end(), 0.0) / bitDifferences.size();
    double minBitDiff = *std::min_element(bitDifferences.begin(), bitDifferences.end());
    double maxBitDiff = *std::max_element(bitDifferences.begin(), bitDifferences.end());
    double avgHexDiff = std::accumulate(hexDifferences.begin(), hexDifferences.end(), 0.0) / hexDifferences.size();
    double minHexDiff = *std::min_element(hexDifferences.begin(), hexDifferences.end());
    double maxHexDiff = *std::max_element(hexDifferences.begin(), hexDifferences.end());

    std::cout << "Bit Differences:\n";
    std::cout << "  Average: " << avgBitDiff << "%\n";
    std::cout << "  Minimum: " << minBitDiff << "%\n";
    std::cout << "  Maximum: " << maxBitDiff << "%\n";
    std::cout << "Hex Differences:\n";
    std::cout << "  Average: " << avgHexDiff << "%\n";
    std::cout << "  Minimum: " << minHexDiff << "%\n";
    std::cout << "  Maximum: " << maxHexDiff << "%\n";
}

void evaluateFileHashes(const string& fileName, bool isSHA256) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    vector<std::array<uint8_t, HASH_SIZE>> hashValues;
    unordered_map<string, vector<string>> hashCollisions;  // Store hash strings as keys, inputs as values
    string line;

    while (getline(inputFile, line)) {
        string hexHash;
        array<uint8_t, HASH_SIZE> hash;

        if (isSHA256) {
            // Hash the line using SHA256 if isSHA256 is true
            SHA256 sha256;
            sha256.update(line);  // Hash the line
            hash = sha256.digest();  // Get the digest
            hexHash = toHexString(hash);  // Convert the hash to a hex string
        } else {
            // The line is expected to be pre-hashed in `results.txt`, so directly use it
            if (line.rfind("Hash: ", 0) == 0) {
                hexHash = line.substr(6);  // Extract hash after "Hash: "
            } else {
                cerr << "Error: Expected a pre-hashed value starting with 'Hash: '." << endl;
                continue;
            }

            // Convert hex string back to byte array for analysis
            for (size_t i = 0; i < HASH_SIZE; ++i) {
                string byteString = hexHash.substr(i * 2, 2);
                hash[i] = static_cast<uint8_t>(stoi(byteString, nullptr, 16));
            }
        }

        hashValues.push_back(hash);  // Store the hash

        // Check for collisions
        if (hashCollisions.find(hexHash) != hashCollisions.end()) {
            hashCollisions[hexHash].push_back(line);  // Add the current line to existing collision entry
        } else {
            hashCollisions[hexHash] = {line};  // Initialize new entry with the current line
        }
    }

    inputFile.close();

    // Analyze differences between consecutive hashes
    analyzeHashDifferences(hashValues);

    // Report collisions
    std::cout << "\nCollision Report:\n";
    bool collisionFound = false;
    for (const auto& entry : hashCollisions) {
        if (entry.second.size() > 1) {  // If multiple inputs have the same hash
            collisionFound = true;
            std::cout << "Hash: " << entry.first << " has collisions for inputs: \n";
            for (const auto& input : entry.second) {
                std::cout << "  " << input << "\n";
            }
        }
    }
    if (!collisionFound) {
        std::cout << "No collisions found.\n";
    }
}

#endif // HASH_EVALUATOR_H
