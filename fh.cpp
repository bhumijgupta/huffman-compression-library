#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <bitset>

#include "src/scanner.h"
#define cfp charFreqPair

using namespace std;

class charFreqPair
{
    char ch;
    int freq;

public:
    cfp *left;
    cfp *right;
    charFreqPair(){};
    charFreqPair(char const &ch, int const &freq) : ch(ch), freq(freq)
    {
        left = NULL;
        right = NULL;
    };
    char getChar() const { return ch; }
    int getFreq() const { return freq; }
    void setFreq(int freq) { this->freq = freq; }
    ~charFreqPair()
    {
        delete left;
        delete right;
    }
};

class pairComparator
{
public:
    // [4]
    int operator()(cfp *const &a, cfp *const &b)
    {
        return a->getFreq() > b->getFreq();
    }
};

void traverse(cfp *head, unordered_map<char, string> &charKeyMap, string s)
{
    if (head->left == NULL && head->right == NULL)
    {
        charKeyMap[head->getChar()] = s;
        return;
    }
    traverse(head->left, charKeyMap, s + "0");
    traverse(head->right, charKeyMap, s + "1");
}

void traverse(cfp *head, unordered_map<string, char> &keyCharMap, string s)
{
    if (head->left == NULL && head->right == NULL)
    {
        keyCharMap[s] = head->getChar();
        return;
    }
    traverse(head->left, keyCharMap, s + "0");
    traverse(head->right, keyCharMap, s + "1");
}

cfp *readTree(ifstream &reader)
{
    char nodeType;
    reader >> noskipws >> nodeType;
    if (nodeType == '1')
    {
        char ch;
        reader >> noskipws >> ch;
        cfp *head = new cfp(ch, 0);
        return head;
    }
    cfp *head = new cfp('~', 0);
    head->left = readTree(reader);
    head->right = readTree(reader);
}

void writeTree(ofstream &writer, cfp *head)
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

void prettyPrint(string out)
{
    cout << left << setw(30) << out;
}
void prettyPrint(int out)
{
    cout << left << setw(30) << out;
}
int main()
{
    // [1]
    ifstream reader;
    // [2]
    reader.open("newfile.txt", ios::in);
    if (!reader)
    {
        cout << "No such file exists";
    }
    else
    {
        // map for index in vector
        unordered_map<char, int> *index = new unordered_map<char, int>;
        vector<cfp *> freq_store;
        char ch;
        int numChars = 0;
        // [3]
        while (reader >> noskipws >> ch)
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
            cout << "No need for encryption\n";
            return 0;
        }
        // Create min priority queue for cfp pair
        priority_queue<cfp *, vector<cfp *>, pairComparator> freq_sorted;
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
        unordered_map<char, string> charKeyMap;
        traverse(head, charKeyMap, "");

        // Read from file and write compressed to new file
        ofstream writer;
        writer.open("newfile_compressed.txt", ios::out | ios::trunc);
        reader.open("newfile.txt", ios::in);

        // First write the tree in preorder form
        writeTree(writer, head);
        delete head;
        // Write numChars to check file integrity
        writer << numChars;
        char chr = 0;
        int bufferSize = 8;
        int size = 0;
        while (reader >> noskipws >> ch)
        {
            string bin = charKeyMap[ch];
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

        // Decompression
        reader.open("newfile_compressed.txt", ios::in);
        // create huffman tree from file
        head = readTree(reader);
        // Create key char map for decompression
        unordered_map<string, char> keyCharMap;
        traverse(head, keyCharMap, "");
        delete head;
        // Read total number of characters
        int totalChars;
        reader >> noskipws >> totalChars;
        writer.open("newfile_original.txt", ios::out | ios::trunc);
        string key = "";
        int readChars = 0;
        while (reader >> noskipws >> ch && readChars != totalChars)
        {
            string bin_read = bitset<8>(ch).to_string();
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

        scanner sc;
        int original_size = numChars;
        int compressed_size = sc.getFileSize("newfile_compressed.txt");
        // If file not present or cannot open, it returns -1
        if (compressed_size == -1)
            return 1;
        int decompressed_size = sc.getFileSize("newfile_original.txt");
        // If file not present or cannot open, it returns -1
        if (decompressed_size == -1)
            return 1;

        cout << "----------Completed---------- \n";
        prettyPrint("Filetype");
        prettyPrint("Filename");
        prettyPrint("Filesize in bytes");
        cout << "\n\n";
        prettyPrint("Original");
        prettyPrint("newfile.txt");
        prettyPrint(original_size);
        cout << "\n";
        prettyPrint("Compressed");
        prettyPrint("newfile_compressed.txt");
        prettyPrint(compressed_size);
        cout << "\n";
        prettyPrint("Decompressed");
        prettyPrint("newfile_original.txt");
        prettyPrint(decompressed_size);
        cout << "\n";
    }
    return 0;
}