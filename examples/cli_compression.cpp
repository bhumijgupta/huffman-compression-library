#include <iostream>
#include <string>
#include "../huffmantool.h"

using namespace std;

void show_help(string const &name)
{
    cerr << "Usage: " << name << " <option(s)> SOURCES\n"
         << "Options:\n"
         << "\t-h,--help\t\tShow this help message\n"
         << "\tbenchmark\t\tBenchmark\n"
         << "compress <source file> [destination file]\t\tCompress source file\n"
         << "decompress <source file> [destination file]\t\tDecompress source file\n"
         << endl;
}

int main(int argc, char const *argv[])
{
    huffmantool ht;
    if (argc < 2 || argc > 4)
    {
        show_help(argv[0]);
    }
    else if (argc == 2)
    {
        string arg = argv[1];
        if (arg == "benchmark")
            ht.benchmark("../sample/newfile.txt");
        else
            show_help(argv[0]);
    }
    else
    {
        string arg1 = argv[1];
        string arg2 = argv[2];
        // ******************************
        // 	COMPRESS
        // ******************************

        if (arg1 == "compress")
        {
            string output;
            if (argc == 3)
                output = ht.compressFile(arg2);
            else
            {
                string arg3 = argv[3];
                output = ht.compressFile(arg2, arg3);
            }
            if (output != "")
            {
                cout << "Output file: " << output << "\n";
            }
        }
        // ******************************
        // 	DECOMPRESS
        // ******************************

        else if (arg1 == "decompress")
        {
            string output;
            if (argc == 3)
                output = ht.decompressFile(arg2);
            else
            {
                string arg3 = argv[3];
                output = ht.decompressFile(arg2, arg3);
            }
            if (output != "")
            {
                cout << "Output file: " << output << "\n";
            }
        }
        else
            show_help(argv[0]);
    }
    return 0;
}
