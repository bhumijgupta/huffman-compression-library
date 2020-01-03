#ifndef CFP_H
#define CFP_H

#include <cstddef>

class charFreqPair
{
    char ch;
    int freq;

public:
    charFreqPair *left;
    charFreqPair *right;
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
    // [6]
    int operator()(charFreqPair *const &a, charFreqPair *const &b)
    {
        return a->getFreq() > b->getFreq();
    }
};

#endif // CFP_H