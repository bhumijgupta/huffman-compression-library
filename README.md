# HCL (Huffman Compression Library)

> HCL a custom C++ library that allows file compression using Huffman Compression algorithm.

[![](https://img.shields.io/badge/Try%20Online-repl.it-brightgreen)](https://repl.it/@bhumijgupta/huffman-demo) ![](https://github.com/bhumijgupta/huffman-compression-library/workflows/Unit%20Test/badge.svg) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/2e33d1fc65044a789e07665a674ad806)](https://www.codacy.com/manual/bhumijgupta/huffman-compression-library?utm_source=github.com&utm_medium=referral&utm_content=bhumijgupta/huffman-compression-library&utm_campaign=Badge_Grade) ![GitHub repo size](https://img.shields.io/github/repo-size/bhumijgupta/huffman-compression-library) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/bhumijgupta/huffman-compression-library) ![GitHub top language](https://img.shields.io/github/languages/top/bhumijgupta/huffman-compression-library)

## Table of Contents

- [Features](#Features)
- [Benchmarks](#Benchmarks)
- [Usage](#Usage)
  - [Try Online](#Try-Online)
  - [Try Locally](#Try-Locally)
- [Methods](#Methods)
  - [compressFile](#compressFile)
  - [decompressFile](#decompressFile)
  - [benchmark](#benchmark)
- [Performance Comparison](#Performance-Comparison)
  - [Conclusion](#Conclusion)
- [Examples](#Examples)
- [Run tests](#Run-tests)
- [License](#License)
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

There are 2 ways to try the HCL library

### 1. Try Online

Try the HCL library online on my custom repl. [![](https://img.shields.io/badge/Try%20Online-repl.it-blue)](https://repl.it/@bhumijgupta/huffman-demo)

### 2. Try Locally

1.  Clone this repo to your project folder  
    `git clone https://github.com/bhumijgupta/huffman-compression-library.git hcl`

2.  Import Huffman tool header and create new `huffmantool` object

```C++
// Filename main.cpp
#include "hcl/huffmantool.h"

int main(){
    huffmantool ht;
    .
    .
    .
}
```

3.  Compile and run file

```bash
g++ main.cpp && ./a.out
```

## Methods

### compressFile

```C++
compressFile(string sourceFile, string compressedFile = "") : string
```

Returns the destination of compressed file.  
If the `sourceFile` provided is invalid or cannot be opened, it returns an empty string.

If no `compressedFile` is provided, `compressedFile` is present at the same dir as `sourcefile` with prefix `compressed_`

### decompressFile

```C++
decompressFile(string compressedFile, string retrievedFile = "") : string
```

Returns the destination of decompressed file.  
If the `compressedFile` provided is invalid or cannot be opened, it reurns an empty string.

If no `retrievedFile` is provided, `retrievedFile` is present at the same dir as `compressedFile` with prefix `decompressed_` and removed `compressed_` prefix (if present)

### benchmark

```C++
benchmark(string sourcefile) : void
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

<b>Note</b>: For detailed understanding of code refer to [Wiki](https://github.com/bhumijgupta/huffman-compression-library/wiki).

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

## Examples

Following are the list of examples

- [CLI tool for file compression](examples/cli_compression.cpp)
- [Benchmarking tool](examples/benchmark.cpp)

## Run tests

![](https://github.com/bhumijgupta/huffman-compression-library/workflows/Unit%20Test/badge.svg)

The tests are stored in `tests/` directory. You need to install [googletest](https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/) framework to run test.

```bash
cd tests/
cmake CMakeLists.txt
make
./runTests
```

## License

![GitHub](https://img.shields.io/github/license/bhumijgupta/huffman-compression-library)

This library is licensed under MIT License. The license file can be found [here](LICENSE)

## Author

[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)

### Bhumij Gupta

![GitHub followers](https://img.shields.io/github/followers/bhumijgupta?label=Follow&style=social) [![LinkedIn](https://img.shields.io/static/v1.svg?label=connect&message=@bhumijgupta&color=success&logo=linkedin&style=flat&logoColor=white)](https://www.linkedin.com/in/bhumijgupta/) ![Twitter URL](https://img.shields.io/twitter/url?style=social&url=http%3A%2F%2Ftwitter.com%2Fbhumijgupta)

---

```C++
if(repo.isAwesome || repo.isHelpful){
    StarRepo();
}
```
