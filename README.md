# File Compression Using Huffman Coding

This is a C++ project to demonstrate file compression and decompression using Huffman Coding Algorithm.

## Screenshot

![output.png](assets/output.png)

## Usage

1. Enter the data you want to be compressed in (sample/newfile.txt)[sample/newfile.txt]
2. Run `g++ fh.cpp && ./a.out`

## Performance Comparison

The `newfile.txt` is populated using lorem ipsum data from [lipsum.com](https://www.lipsum.com/) and the following data is noticed.

| Number of characters | Original file size (in bytes) | Compressed file size (in bytes) |
| :------------------: | :---------------------------: | :-----------------------------: |
|          10          |              10               |               36                |
|         100          |              100              |               116               |
|         1000         |             1000              |               642               |
|        10000         |             10000             |              5445               |
|        100000        |            100000             |              53354              |

## Conclusion

The compression ratio and performance of the Huffman coding depends on the size of input text and the frequency of distinct characters in the file. From the above comparison we can see that the current implementation of huffman coding does not produce positive compression ratio for small files due to the overhead of storing `Huffman Tree` in the compressed file, which is useful at the time of decompression.

But we notice, as the size of file increases (>=1000 bytes), the compression ratio stays at almost 50%.
