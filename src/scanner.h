#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <fstream>
#include <string>
class scanner
{
    std::ifstream reader;

public:
    int getFileSize(std::string filename)
    {
        try
        {
            reader.open(filename, std::ios::binary | std::ios::ate);
            if (!reader.is_open())
                throw "ERROR: Cannot open file " + filename;
            reader.seekg(0, std::ios::end);
            int size = reader.tellg();
            reader.close();
            return size;
        }
        catch (std::string err)
        {
            std::cerr << err << "\n";
            return -1;
        }
    }
};

#endif // SCANNER_H