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

#include "src/scanner.h"
#include "src/cfp.h"

class huffmantool
{
    void traverse(cfp *head, std::unordered_map<char, std::string> &charKeyMap, std::string s)
    {
        if (head->left == NULL && head->right == NULL)
        {
            charKeyMap[head->getChar()] = s;
            return;
        }
        traverse(head->left, charKeyMap, s + "0");
        traverse(head->right, charKeyMap, s + "1");
    }

    void traverse(cfp *head, std::unordered_map<std::string, char> &keyCharMap, std::string s)
    {
        if (head->left == NULL && head->right == NULL)
        {
            keyCharMap[s] = head->getChar();
            return;
        }
        traverse(head->left, keyCharMap, s + "0");
        traverse(head->right, keyCharMap, s + "1");
    }

    cfp *readTree(std::ifstream &reader)
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
    }

    void writeTree(std::ofstream &writer, cfp *head)
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

    void prettyPrint(std::string out)
    {
        std::cout << std::left << std::setw(30) << out;
    }
    void prettyPrint(int out)
    {
        std::cout << std::left << std::setw(30) << out;
    }
    void printSeparator()
    {
        for (int i = 0; i < 80; i++)
            std::cout << "-";
        std::cout << "\n";
    }

    int lposSlash(std::string filename)
    {
        int pos = -1;
        for (int i = 0; i < filename.length(); i++)
        {
            if (filename[i] == '/')
                pos = i;
        }
        return pos;
    }

public:
    std::string compressFile(std::string sourcefile, std::string compressedFile = "")
    {
        if (compressedFile == "")
        {
            int pos = lposSlash(sourcefile);
            compressedFile = sourcefile.substr(0, pos + 1) + "compressed_" + sourcefile.substr(pos + 1);
        }
        // [1]
        std::ifstream reader;
        // [2]
        reader.open(sourcefile, std::ios::in);
        if (!reader.is_open())
        {
            std::cout << "No such file exists or cannot open file " + sourcefile;
            return "";
        }
        // map for index in vector
        std::unordered_map<char, int> *index = new std::unordered_map<char, int>;
        std::vector<cfp *> freq_store;
        char ch;
        int numChars = 0;
        // [3]
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
            std::cout << "No need for encryption\n";
            return 0;
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
        reader.open(sourcefile, std::ios::in);

        // First write the tree in preorder form
        writeTree(writer, head);
        delete head;
        // Write numChars to check file integrity
        writer << numChars;
        char chr = 0;
        int bufferSize = 8;
        int size = 0;
        while (reader >> std::noskipws >> ch)
        {
            std::string bin = charKeyMap[ch];
            for (int i = 0; i < bin.length(); i++)
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
    std::string decompressFile(std::string compressedFile, std::string retrievedFile = "")
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
            std::string bin_read = std::bitset<8>(ch).to_string();
            for (int i = 0; i < bin_read.length(); i++)
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
        return retrievedFile;
    };
    void benchmark(std::string sourcefile = "sample/newfile.txt")
    {
        auto start_compression = std::chrono::high_resolution_clock::now();
        std::string compressedFile = compressFile(sourcefile);
        auto end_compression = std::chrono::high_resolution_clock::now();
        if (compressedFile == "")
            return;
        auto compression_time = std::chrono::duration_cast<std::chrono::microseconds>(end_compression - start_compression);

        auto start_decompression = std::chrono::high_resolution_clock::now();
        std::string retrievedFile = decompressFile(compressedFile);
        auto end_decompression = std::chrono::high_resolution_clock::now();
        auto decompression_time = std::chrono::duration_cast<std::chrono::microseconds>(end_decompression - start_decompression);

        // Get file sizes
        scanner sc;
        int original_size = sc.getFileSize(sourcefile);
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
        sourcefile = sourcefile.substr(lposSlash(sourcefile) + 1);
        prettyPrint(sourcefile);
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

        float compression = ((float)compressed_size / original_size) * 100.0;
        std::cout << "\n\n";
        printSeparator();
        std::cout << "Time taken to compress file: " << compression_time.count() << " microseconds\n";
        std::cout << "Time taken to decompress file: " << decompression_time.count() << " microseconds\n";
        std::cout << "Compression: " << compression << "% \n\n";
    };
};

#endif // HUFFMAN_TOOL_H