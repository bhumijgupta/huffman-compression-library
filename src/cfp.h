#define cfp charFreqPair
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