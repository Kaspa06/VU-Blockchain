#include <iostream>
#include <bitset>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;

// Function to apply transformations to make inputs look more different
string modifyInput(string& input) {
    for (size_t i = 0; i < input.size(); ++i) {
        if (isupper(input[i]) || input[i] == '!') {
            input[i] ^=(i % 10);  // XOR with position modulo 10
            input[i] +=5;         // Shift ASCII value up by 5
        } else {
            input[i] ^=(i + 3);  // XOR with position plus 3
            input[i] -=2;        // Shift ASCII value down by 2
        }
    }

    // Reverse the string to add more differences
    reverse(input.begin(), input.end());

    return input;
}

// Function to convert input string to 256-bit binary
string inputToBits(string& input) {
    string bits;

    while (input.length() < 32) {
        input += 'k'; // might even call it SALT in case the input is too short
    }

    // Convert each character to its 8-bit binary representation
    for (char c : input) {
        bits += bitset<8>(static_cast<unsigned char>(c)).to_string();
    }

    return bits;
}

// Function to increment hexadecimal characters
char incrementHexChar(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') {
        return (hexChar == '9') ? '0' : (hexChar + 1);
    } else if (hexChar >= 'a' && hexChar <= 'f') {
        return (hexChar == 'f') ? 'a' : (hexChar + 1);
    }
    return hexChar;
}

// Function to convert bits to hexadecimal hash
string bitsToHex(string& bits, const string& originalInput) {
    // Pad the binary string to be a multiple of 4
    while (bits.size() % 4 != 0) {
        bits = '0' + bits;
    }

    stringstream hexStream;
    bool containsUpper = any_of(originalInput.begin(), originalInput.end(), ::isupper);
    bool containsExclamation = originalInput.find('!') != string::npos;

    // Determine multiplier base
    int multiplierBase = 1;
    if (containsUpper) {
        multiplierBase = originalInput.length(); //Random logic to radically change the output for UpperCase
    } else if (containsExclamation) {
        multiplierBase = originalInput.length() / 2; //Random logic to radically change the output for !
    }

    int rollingValue = 1;

    // Convert 4-bit chunks to hexadecimal
    for (size_t i = 0; i < bits.size(); i += 4) {
        string chunk = bits.substr(i, 4);
        int value = stoi(chunk, nullptr, 2);

        // Apply transformations
        int charInfluence = originalInput[i / 4] % 16; // i/4 selects every character, % 16 is ensuring that the influence is constrained between 0 and 15
        rollingValue += (charInfluence + 1);

        // make the value less predictable and more complex
        value *= rollingValue;
        value *= multiplierBase + (i / 4); //ensuring that longer inputs and higher indices lead to greater modifications
        value *= abs(sin(originalInput.length() + i / 4)) * 100; //Adding non-linearity to the transformation

        hexStream << hex << value;
    }

    string hexResult = hexStream.str();

    // Modify each hexadecimal character
    for (char& c : hexResult) {
        c = incrementHexChar(c);
    }

    // Ensure the result is exactly 64 characters
    if (hexResult.length() > 64) {
        hexResult = hexResult.substr(0, 64);
    } else {
        while (hexResult.length() < 64) {
            hexResult += '0';
        }
    }

    return hexResult;
}

// Function to compute word sum
int computeWordSum(const string& input) {
    int sum = 0;

    // Calculate the sum of alphabetical positions for each character (ignoring non-alphabetical characters)
    for (char c : input) {
        if (isalpha(c)) {
            // Convert to lowercase for consistent calculation
            sum += tolower(c) - 'a' + 1;
        }
    }

    return sum;
}

// Function to multiply bits by word sum
string multiplyBitsByWordSum(string bits, int wordSum) {
    // Iterate over each bit and multiply its numeric value by wordSum
    for (size_t i = 0; i < bits.size(); ++i) {
        if (bits[i] == '1') {
            bits[i] = (wordSum % 2) ? '1' : '0';
        } else {
            bits[i] = (wordSum % 2) ? '0' : '1';
        }
        wordSum /= 2; // Update wordSum for the next bit, scaling down
        if (wordSum == 0) break; // If wordSum is exhausted, stop modifying the bits
    }

    return bits;
}

// Function to process input and generate the hash
void processInput(const string& input, ofstream& outputFile) {
    string modifiedInput = input + to_string(input.length()); // Salt the input by appending its length

    modifyInput(modifiedInput);
    string binaryResult = inputToBits(modifiedInput);
    int wordSum = computeWordSum(modifiedInput);
    string modifiedBits = multiplyBitsByWordSum(binaryResult, wordSum);
    string hashResult = bitsToHex(modifiedBits, modifiedInput);

    // Output the hash
    outputFile << "Hash: " << hashResult << endl;
}

int main() {
    int choice;
    cout << "Select input method: " << endl;
    cout << "1. Input by hand" << endl;
    cout << "2. Input from file (konstitucija.txt)" << endl;
    cin >> choice;

    ofstream outputFile("results.txt");

    if (!outputFile.is_open()) {
        cout << "Error opening output file!" << endl;
        return 1;
    }

    if (choice == 1) {
        string input;
        cout << "Enter input: ";
        cin.ignore(); // To ignore the newline character left in the input buffer
        getline(cin, input);

        processInput(input, outputFile); // Process the input and write to file

    } else if (choice == 2) {
        ifstream inputFile("konstitucija.txt");

        if (!inputFile.is_open()) {
            cout << "Error opening input file!" << endl;
            return 1;
        }

        string line;
        while (getline(inputFile, line)) {
            processInput(line, outputFile); // Process each line and write to file
        }

        inputFile.close();
    } else {
        cout << "Invalid choice!" << endl;
    }

    outputFile.close();
    cout << "Hash results saved to results.txt" << endl;

    return 0;
}
