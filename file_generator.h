#ifndef FILE_GENERATOR_H
#define FILE_GENERATOR_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

class FileGenerator {
public:
    static void createRandomStringPairsFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            generatePairs(file, 12500, 10);    
            generatePairs(file, 12500, 100);   
            generatePairs(file, 12500, 500);   
            generatePairs(file, 12500, 1000);  
            file.close();
            std::cout << "File " << filename << " created with 100,000 strings" << std::endl;
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }

    static void createRandomStringPairsWithOneDifference(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            generatePairsWithDifference(file, 12500, 10);    
            generatePairsWithDifference(file, 12500, 100);   
            generatePairsWithDifference(file, 12500, 500);   
            generatePairsWithDifference(file, 12500, 1000);  
            file.close();
            std::cout << "File " << filename << " created with 100,000 strings" << std::endl;
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }

private:
    static void generatePairs(std::ofstream& file, int numPairs, int length) {
        for (int i = 0; i < numPairs; ++i) {
            std::string str1, str2;
            for (int j = 0; j < length; ++j) {
                char randomChar1 = 'a' + rand() % 26;
                char randomChar2 = 'a' + rand() % 26; 
                str1 += randomChar1;
                str2 += randomChar2;
            }
            file << str1 << std::endl;
            file << str2 << std::endl;
        }
    }

    static void generatePairsWithDifference(std::ofstream& file, int numPairs, int length) {
        for (int i = 0; i < numPairs; ++i) {
            std::string str1, str2;
            for (int j = 0; j < length; ++j) {
                char randomChar = 'a' + rand() % 26;
                str1 += randomChar;
                str2 += randomChar;
            }
           
            int diffIndex = rand() % length;
            str2[diffIndex] = (str2[diffIndex] == 'a') ? 'b' : 'a';

            file << str1 << std::endl;
            file << str2 << std::endl;
        }
    }

public:

    static void createSingleCharFile(const std::string& filename, char symbol) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << symbol;
            file.close();
            std::cout << "File " << filename << " created with symbol: " << symbol << std::endl;
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }

    static void createRandomCharFile(const std::string& filename, int length) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < length; ++i) {
                char randomChar = 'a' + rand() % 26; // Generating random lowercase letters
                file << randomChar;
            }
            file.close();
            std::cout << "File " << filename << " created with " << length << " random characters." << std::endl;
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }

    static void createOneDifferenceFile(const std::string& filename1, const std::string& filename2, int length, int diffIndex) {
        std::ofstream file1(filename1), file2(filename2);
        if (file1.is_open() && file2.is_open()) {
            char commonChar = 'a' + rand() % 26; // Common random char
            for (int i = 0; i < length; ++i) {
                char randomChar = 'a' + rand() % 26;
                file1 << randomChar;
                if (i == diffIndex) {
                    file2 << (randomChar == commonChar ? randomChar + 1 : commonChar); // Make the char different at diffIndex
                } else {
                    file2 << randomChar;
                }
            }
            file1.close();
            file2.close();
            std::cout << "Files " << filename1 << " and " << filename2 << " created with one character difference at index " << diffIndex << std::endl;
        } else {
            std::cerr << "Unable to open files: " << filename1 << " or " << filename2 << std::endl;
        }
    }

    static void createEmptyFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file.close();
            std::cout << "Empty file " << filename << " created." << std::endl;
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }
};

#endif // FILE_GENERATOR_H
