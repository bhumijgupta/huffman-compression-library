# Huffman Compression Library

This is a custom C++ library that implements Huffman coding compression algorithm. The library exposes methods that provides high level abstraction over compression and decompression of files.  
[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)  
![](https://github.com/actions/hello-world/workflows/.github/workflows/ccpp.yml/badge.svg) ![GitHub repo size](https://img.shields.io/github/repo-size/bhumijgupta/huffman-compression-library) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/bhumijgupta/![GitHub repo size](https://img.shields.io/github/repo-size/bhumijgupta/huffman-compression-library)) ![GitHub top language](https://img.shields.io/github/languages/top/bhumijgupta/huffman-compression-library)

## Table of Contents

- [Features](#Features)
- [Benchmarks](#Benchmarks)
- [Usage](#Usage)
- [Methods](#Methods)
  - [compressFile](#compressFile)
  - [decompressFile](#decompressFile)
  - [benchmark](#benchmark)
- [Performance Comparison](#Performance-Comparison)
  - [Conclusion](#Conclusion)
- [Run tests](#Run-tests)
- [Author](#Author)

## Features

- Fast and efficient
- Supports all kinds of files (images, pdf, video, text, etc)
- High level abstraction methods to be used
- No need of understanding how compression algorithm works
- Light weight library
- Compression ratio of upto 50%

## Benchmarks

Below is the output for `huffmantool.benchmark()` method
![output.png](assets/output.png)

## Usage

1. Clone this repo to your project folder
   `git clone git@github.com:bhumijgupta/huffman-compression-library.git htlibrary`

2. Import Huffman tool header and create new `huffmantool` object

```C++
// Filename main.cpp
#import "htlibrary/huffmantool.h"

int main(){
    huffmantool ht;
    .
    .
    .
}
```

2. Compile and run file

```bash
g++ main.cpp && ./a.out
```

## Methods

### compressFile

```C++
compressFile(string sourcefile, string compressedFile = "") : string
```

Returns the destination of compressed file.  
If no `compressedFile` is provided, `compressedFile` is present at the same dir as `sourcefile` with prefix `compressed_`

### decompressFile

```C++
decompressFile(string compressedFile, string retrievedFile = "") : string
```

Returns the destination of decompressed file.  
If no `retrievedFile` is provided, `retrievedFile` is present at the same dir as `compressedFile` with prefix `decompressed_` and removed `compressed_` prefix (if present)

### benchmark

```C++
benchmark(string sourcefile = "sample/newfile.txt") : void
```

This method performs compression and decompression on the sourcefile provided and prints the following stats to the stdout.

```
--------------------------------------------------------------------------------
                              B E N C H M A R K
--------------------------------------------------------------------------------

Filetype                      Filename                      Filesize in bytes

Original
Compressed
Decompressed

--------------------------------------------------------------------------------
Time taken to compress file: XYZ microseconds
Time taken to decompress file: XYZ microseconds
Compression: XY.Z%

```

If no `sourcefile` is given, source file is considered as `sample/newfile.txt`

## Performance Comparison

The `newfile.txt` is populated using lorem ipsum data from [lipsum.com](https://www.lipsum.com/) and the following data is noticed after executing `main.cpp`.

| Number of characters | Original file size (in bytes) | Compressed file size (in bytes) |
| :------------------: | :---------------------------: | :-----------------------------: |
|          10          |              10               |               36                |
|         100          |              100              |               116               |
|         1000         |             1000              |               642               |
|        10000         |             10000             |              5445               |
|        100000        |            100000             |              53354              |

### Conclusion

The compression ratio and performance of the Huffman coding depends on the size of input text and the frequency of distinct characters in the file. From the above comparison we can see that the current implementation of huffman coding does not produce positive compression ratio for small files due to the overhead of storing `Huffman Tree` in the compressed file, which is useful at the time of decompression.

But we notice, as the size of file increases (>=1000 bytes), the compression ratio stays at almost 50%.

## Run tests

The tests are stored in `tests/` directory. You need to install [gtest](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/) framework to run test.

```bash
cmake CMakeLists.txt
make
./runTests
```

## Future works

- Add more unit tests
- Explain in detail the working and implemetation in wiki
- Add more examples
- Add better documentation

## Author

[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)

### Bhumij Gupta

![GitHub followers](https://img.shields.io/github/followers/bhumijgupta?label=Follow&style=social) [![LinkedIn](https://img.shields.io/static/v1.svg?label=connect&message=@bhumijgupta&color=success&logo=linkedin&style=flat&logoColor=white)](https://www.linkedin.com/in/bhumijgupta/) ![Twitter URL](https://img.shields.io/twitter/url?style=social&url=http%3A%2F%2Ftwitter.com%2Fbhumijgupta)
