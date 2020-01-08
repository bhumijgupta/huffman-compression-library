// The MIT License (MIT)

// Copyright (c) 2020 BHUMIJ GUPTA

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

/* code */

// [1] See Wiki for more info
#ifndef HUFFMAN_TOOL_H
#define HUFFMAN_TOOL_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <bitset>
#include <chrono>

// ******************************
// 	CUSTOM INCLUDES & MACROS
// ******************************

#include "src/scanner.h"
#include "src/cfp.h"

#define cfp charFreqPair

// ******************************
// 	CLASS DECLARATION
// ******************************

/**
 * @brief This class contains methods for file compression and decompression
 * 
 */
class huffmantool
{
    void traverse(cfp const *, std::unordered_map<char, std::string> &, std::string const);
    void traverse(cfp const *, std::unordered_map<std::string, char> &, std::string const);
    cfp *readTree(std::ifstream &);
    void writeTree(std::ofstream &, cfp const *);
    void prettyPrint(std::string const);
    void prettyPrint(int const);
    void printSeparator();
    int lposSlash(std::string const);

public:
    std::string compressFile(std::string, std::string);
    std::string decompressFile(std::string, std::string);
    void benchmark(std::string);
};

// ******************************
// 	METHODS DEFINATION
// ******************************

void huffmantool::traverse(cfp const *head, std::unordered_map<char, std::string> &charKeyMap, std::string const s)
{
    if (head->left == NULL && head->right == NULL)
    {
        charKeyMap[head->getChar()] = s;
        return;
    }
    traverse(head->left, charKeyMap, s + "0");
    traverse(head->right, charKeyMap, s + "1");
}

void huffmantool::traverse(cfp const *head, std::unordered_map<std::string, char> &keyCharMap, std::string const s)
{
    if (head->left == NULL && head->right == NULL)
    {
        keyCharMap[s] = head->getChar();
        return;
    }
    traverse(head->left, keyCharMap, s + "0");
    traverse(head->right, keyCharMap, s + "1");
}

cfp *huffmantool::readTree(std::ifstream &reader)
{
    char nodeType;
    reader >> std::noskipws >> nodeType;
    if (nodeType == '1')
    {
        char ch;
        reader >> std::noskipws >> ch;
        cfp *head = new cfp(ch, 0);
        return head;
    }
    cfp *head = new cfp('~', 0);
    head->left = readTree(reader);
    head->right = readTree(reader);
    return head;
}

void huffmantool::writeTree(std::ofstream &writer, cfp const *head)
{
    if (head->left == NULL && head->right == NULL)
    {
        writer << '1';
        writer << head->getChar();
        return;
    }
    writer << '0';
    writeTree(writer, head->left);
    writeTree(writer, head->right);
}

void huffmantool::prettyPrint(std::string const out)
{
    // [5]
    std::cout << std::left << std::setw(30) << out;
}
void huffmantool::prettyPrint(int const out)
{
    std::cout << std::left << std::setw(30) << out;
}
void huffmantool::printSeparator()
{
    for (int i = 0; i < 80; i++)
        std::cout << "-";
    std::cout << "\n";
}

int huffmantool::lposSlash(std::string const filename)
{
    int pos = -1;
    for (unsigned int i = 0; i < filename.length(); i++)
    {
        if (filename[i] == '/')
            pos = i;
    }
    return pos;
}

/**
 * @brief Compress the given file
 * 
 * @param sourceFile Path of the file to be compressed (relative or absolute)
 * @param compressedFile Path of the compressed file. Default: path/"compressed_"+sourceFile.extension
 * @return std::string Returns the path of the compressed file. If some error occurs, returns empty string
 */
std::string huffmantool::compressFile(std::string sourceFile, std::string compressedFile = "")
{
    if (compressedFile == "")
    {
        int pos = lposSlash(sourceFile);
        compressedFile = sourceFile.substr(0, pos + 1) + "compressed_" + sourceFile.substr(pos + 1);
    }
    std::ifstream reader;
    reader.open(sourceFile, std::ios::in);
    if (!reader.is_open())
    {
        std::cerr << "ERROR: No such file exists or cannot open file " + sourceFile;
        return "";
    }
    // map for index in vector
    std::unordered_map<char, int> *index = new std::unordered_map<char, int>;
    std::vector<cfp *> freq_store;
    char ch;
    int numChars = 0;
    // [2]
    while (reader >> std::noskipws >> ch)
    {
        numChars++;
        if (index->count(ch) > 0)
        {
            int ind = index->at(ch);
            freq_store[ind]->setFreq(freq_store[ind]->getFreq() + 1);
        }
        else
        {
            index->insert({ch, freq_store.size()});
            freq_store.push_back(new cfp(ch, 1));
        }
    }
    reader.close();
    delete index;
    if (freq_store.size() <= 1)
    {
        std::cout << "INFO: No need for encryption\n";
        return "";
    }
    // Create min priority queue for cfp pair
    std::priority_queue<cfp *, std::vector<cfp *>, pairComparator> freq_sorted;
    for (auto i : freq_store)
    {
        freq_sorted.push(i);
    }
    cfp *head;
    while (!freq_sorted.empty())
    {
        cfp *first = freq_sorted.top();
        freq_sorted.pop();
        cfp *second = freq_sorted.top();
        freq_sorted.pop();
        cfp *newPair = new cfp('~', first->getFreq() + second->getFreq());
        newPair->left = first;
        newPair->right = second;
        freq_sorted.push(newPair);
        if (freq_sorted.size() == 1)
        {
            head = newPair;
            break;
        }
    }
    std::unordered_map<char, std::string> charKeyMap;
    traverse(head, charKeyMap, "");

    // Read from file and write compressed to new file
    std::ofstream writer;
    writer.open(compressedFile, std::ios::out | std::ios::trunc);
    reader.open(sourceFile, std::ios::in);

    // First write the tree in preorder form
    writeTree(writer, head);
    delete head;
    // Write numChars to check file integrity
    writer << numChars;
    char chr = 0;
    int bufferSize = 8;
    while (reader >> std::noskipws >> ch)
    {
        std::string bin = charKeyMap[ch];
        for (unsigned int i = 0; i < bin.length(); i++)
        {
            chr = (chr << 1) ^ (bin[i] - '0');
            bufferSize--;
            if (bufferSize == 0)
            {
                writer << chr;
                chr = 0;
                bufferSize = 8;
            }
        }
    }
    if (bufferSize)
    {
        chr = chr << bufferSize;
        writer << chr;
    }
    writer.close();
    reader.close();
    return compressedFile;
}

/**
 * @brief Decompress the given file
 * 
 * @param compressedFile Path of the file to be decompressed (absolute or relative)
 * @param retrievedFile Path of the decompressed file. Default: path/"decompressed_"+compressedFile.extension
 * @return std::string Returns the path of the retrieved file. If some error occurs, returns empty string
 */
std::string huffmantool::decompressFile(std::string compressedFile, std::string retrievedFile = "")
{
    if (retrievedFile == "")
    {
        int pos = lposSlash(compressedFile);
        retrievedFile = compressedFile.substr(0, pos + 1) + "decompressed_";
        if (compressedFile.length() - pos + 1 >= 11 && compressedFile.substr(pos + 1, 11) == "compressed_")
            retrievedFile += compressedFile.substr(pos + 11 + 1);
        else
            retrievedFile += compressedFile.substr(pos + 1);
    }
    std::ifstream reader;
    std::ofstream writer;
    reader.open(compressedFile, std::ios::in);
    if (!reader.is_open())
    {
        std::cerr << "ERROR: No such file exists or cannot open file " + compressedFile;
        return "";
    }
    // create huffman tree from file
    cfp *head = readTree(reader);
    // Create key char map for decompression
    std::unordered_map<std::string, char> keyCharMap;
    traverse(head, keyCharMap, "");
    delete head;
    // Read total number of characters
    int totalChars;
    reader >> std::noskipws >> totalChars;
    writer.open(retrievedFile, std::ios::out | std::ios::trunc);
    std::string key = "";
    int readChars = 0;
    char ch;
    while (reader >> std::noskipws >> ch && readChars != totalChars)
    {
        // [3]
        std::string bin_read = std::bitset<8>(ch).to_string();
        for (unsigned int i = 0; i < bin_read.length(); i++)
        {
            key += bin_read[i];
            if (keyCharMap.count(key) > 0)
            {
                writer << keyCharMap[key];
                key = "";
                readChars++;
                if (readChars == totalChars)
                    break;
            }
        }
    }
    reader.close();
    writer.close();
    if (readChars != totalChars)
    {
        std::cerr << "ERROR: Compressed file is corrupted\n";
        return "";
    }
    return retrievedFile;
};

/**
 * @brief Benchmarks the performance of the tool. Returns time taken for each step and compression ratio achieved.
 * 
 * @param sourceFile Path of the file to be used as source for compression and decompression
 */
void huffmantool::benchmark(std::string sourceFile)
{
    // [4]
    auto start_compression = std::chrono::high_resolution_clock::now();
    std::string compressedFile = compressFile(sourceFile);
    auto end_compression = std::chrono::high_resolution_clock::now();
    if (compressedFile == "")
        return;
    auto compression_time = std::chrono::duration_cast<std::chrono::microseconds>(end_compression - start_compression);

    auto start_decompression = std::chrono::high_resolution_clock::now();
    std::string retrievedFile = decompressFile(compressedFile);
    if (retrievedFile == "")
        return;
    auto end_decompression = std::chrono::high_resolution_clock::now();
    auto decompression_time = std::chrono::duration_cast<std::chrono::microseconds>(end_decompression - start_decompression);

    // Get file sizes
    scanner sc;
    int original_size = sc.getFileSize(sourceFile);
    if (original_size == -1)
        return;
    int compressed_size = sc.getFileSize(compressedFile);
    // If file not present or cannot open, it returns -1
    if (compressed_size == -1)
        return;
    int decompressed_size = sc.getFileSize(retrievedFile);
    // If file not present or cannot open, it returns -1
    if (decompressed_size == -1)
        return;

    printSeparator();
    std::cout << "                              B E N C H M A R K\n";
    printSeparator();
    std::cout << "\n";

    prettyPrint("Filetype");
    prettyPrint("Filename");
    prettyPrint("Filesize in bytes");
    std::cout << "\n\n";
    prettyPrint("Original");
    sourceFile = sourceFile.substr(lposSlash(sourceFile) + 1);
    prettyPrint(sourceFile);
    prettyPrint(original_size);
    std::cout << "\n";
    prettyPrint("Compressed");
    compressedFile = compressedFile.substr(lposSlash(compressedFile) + 1);
    prettyPrint(compressedFile);
    prettyPrint(compressed_size);
    std::cout << "\n";
    prettyPrint("Decompressed");
    retrievedFile = retrievedFile.substr(lposSlash(retrievedFile) + 1);
    prettyPrint(retrievedFile);
    prettyPrint(decompressed_size);

    float compression = 100.0 - ((float)compressed_size / original_size) * 100.0;
    std::cout << "\n\n";
    printSeparator();
    std::cout << "Time taken to compress file: " << compression_time.count() << " microseconds\n";
    std::cout << "Time taken to decompress file: " << decompression_time.count() << " microseconds\n";
    std::cout << "Compression: " << compression << "% \n\n";
};

#endif // HUFFMAN_TOOL_H