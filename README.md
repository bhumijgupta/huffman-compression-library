# Huffman Compression Library

This is a custom C++ library that implements Huffman coding compression algorithm. The library exposes methods that provides high level abstraction over compression and decompression of files.

## Features

- Fast and efficient
- Supports all kinds of files (images, pdf, video, text, etc)
- High level abstraction methods to be used
- No need of understanding how compression algorithm works
- Light weight library
- Compression ratio of upto 50%

## Screenshot

Below is the output for `huffmantool.benchmark()` method
![output.png](assets/output.png)

## Usage

1. Import Huffman tool header and create new `huffmantool` object

```C++
// Filename main.cpp
#import "huffmantool.h"

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

## Conclusion

The compression ratio and performance of the Huffman coding depends on the size of input text and the frequency of distinct characters in the file. From the above comparison we can see that the current implementation of huffman coding does not produce positive compression ratio for small files due to the overhead of storing `Huffman Tree` in the compressed file, which is useful at the time of decompression.

But we notice, as the size of file increases (>=1000 bytes), the compression ratio stays at almost 50%.
