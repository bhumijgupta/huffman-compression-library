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
    char filename_char[filename.length() + 1];
    strcpy(filename_char, filename.c_str());
    remove(filename_char);
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
    ASSERT_EQ(sc.getFileSize("non_existing_file.txt"), -1);
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

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}