#include "../src/scanner.h"
#include "../huffmantool.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

// ******************************
// 	FUNCTIONS
// ******************************

void removeFile(string filename)
{
    // Check if file exists
    ifstream f;
    f.open(filename);
    if (!f.is_open())
        throw("File not present");
    else
    {
        char filename_char[filename.length() + 1];
        strcpy(filename_char, filename.c_str());
        remove(filename_char);
    }
}

// ******************************
// 	FIXTURES
// ******************************
class TempFile : public ::testing::Test
{
protected:
    char filename[18] = "existing_file.txt";
    void SetUp()
    {
        ofstream fcreate;
        fcreate.open(filename);
        fcreate << "Hello World";
        fcreate.close();
    }
    void TearDown()
    {
        remove(filename);
    }
};

// ******************************
// 	SRC/SCANNER.H
// ******************************

TEST(ScannerTest, Scanner_invalidFile)
{
    scanner sc;
    testing::internal::CaptureStderr();
    ASSERT_EQ(sc.getFileSize("non_existing_file.txt"), -1);
    string stderr = testing::internal::GetCapturedStderr();
    ASSERT_EQ("ERROR: Cannot open file non_existing_file.txt\n", stderr);
}

TEST_F(TempFile, Scanner_validFile)
{
    char filename[] = "existing_file.txt";
    scanner sc;
    ASSERT_EQ(sc.getFileSize(filename), 11);
}

// ******************************
// 	HUFFMANTOOL.H
// ******************************
TEST_F(TempFile, huffman_compressFile_noOutputFile)
{
    huffmantool ht;
    string output = ht.compressFile("existing_file.txt");
    ASSERT_EQ(output, "compressed_existing_file.txt");
    removeFile(output);
}

TEST_F(TempFile, huffman_compressFile_withOutputFile)
{
    huffmantool ht;
    string output = ht.compressFile("existing_file.txt", "com_existing_file.txt");
    ASSERT_EQ(output, "com_existing_file.txt");
    removeFile(output);
}

TEST(huffman_compressFile, invalidFile)
{
    huffmantool ht;
    testing::internal::CaptureStderr();
    string output = ht.compressFile("non_existing_file.txt");
    string stderr = testing::internal::GetCapturedStderr();
    ASSERT_EQ(output, "");
    ASSERT_EQ(stderr, "ERROR: No such file exists or cannot open file non_existing_file.txt");
}

TEST(huffman_compressFile, no_encryption_needed)
{
    // Test setup
    ofstream f;
    f.open("existing_file.txt");
    f << "aaaaaaaaaaa";
    f.close();
    testing::internal::CaptureStdout();

    huffmantool ht;
    ASSERT_EQ(ht.compressFile("existing_file.txt"), "");

    // Test cleardown
    string stdout = testing::internal::GetCapturedStdout();
    ASSERT_EQ(stdout, "INFO: No need for encryption\n");
    removeFile("existing_file.txt");
}

TEST_F(TempFile, huffman_benchmark)
{
    huffmantool ht;
    ASSERT_NO_FATAL_FAILURE(ht.benchmark("existing_file.txt"));
    removeFile("decompressed_existing_file.txt");
    removeFile("compressed_existing_file.txt");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}