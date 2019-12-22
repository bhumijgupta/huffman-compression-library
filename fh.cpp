#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <bitset>
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
        // [3]
        while (reader >> noskipws >> ch)
        {
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
        delete head;

        unordered_map<string, char> keyCharMap;
        for (auto i : charKeyMap)
        {
            keyCharMap[i.second] = i.first;
        }

        ofstream writer;
        writer.open("newfile_compressed.txt", ios::out | ios::trunc);
        reader.open("newfile.txt", ios::in);

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
        if (chr)
        {
            writer << chr;
        }
        writer.close();
        reader.close();

        reader.open("newfile_compressed.txt", ios::in);
        writer.open("newfile_original.txt", ios::out | ios::trunc);
        string key = "";
        while (reader >> noskipws >> ch)
        {
            string bin_read = bitset<8>(ch).to_string();
            for (int i = 0; i < bin_read.length(); i++)
            {
                key += bin_read[i];
                if (keyCharMap.count(key) > 0)
                {
                    writer << keyCharMap[key];
                    key = "";
                }
            }
        }
        reader.close();
        writer.close();

        ifstream scanner;
        scanner.open("newfile.txt", ios::binary | ios::ate);
        scanner.seekg(0, ios::end);
        int original_size = scanner.tellg();
        scanner.close();

        scanner.open("newfile_compressed.txt", ios::binary | ios::ate);
        scanner.seekg(0, ios::end);
        int compressed_size = scanner.tellg();
        scanner.close();

        scanner.open("newfile.txt", ios::binary | ios::ate);
        scanner.seekg(0, ios::end);
        int decompressed_size = scanner.tellg();
        scanner.close();

        cout << "----------Completed---------- \n";
        cout << left << setw(30) << "Filetype";
        cout << left << setw(30) << "Filename";
        cout << left << setw(30) << "Filesize(in bytes)"
             << "\n";
        cout << "\n";
        cout << left << setw(30) << "Original";
        cout << left << setw(30) << "newfile.txt";
        cout << left << setw(30) << original_size << "\n";

        cout << left << setw(30) << "Compressed";
        cout << left << setw(30) << "newfile_compressed.txt";
        cout << left << setw(30) << compressed_size << "\n";

        cout << left << setw(30) << "Decompressed";
        cout << left << setw(30) << "newfile_original.txt";
        cout << left << setw(30) << decompressed_size << "\n";
    }
    return 0;
}