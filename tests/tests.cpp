#include "../src/scanner.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

TEST(ScannerTest, invalidFile)
{
    scanner sc;
    ASSERT_EQ(sc.getFileSize("non_existing_file.txt"), -1);
}
class Scanner_fix : public ::testing::Test
{
protected:
    char filename[18] = "existing_file.txt";
    Scanner_fix()
    {
        ofstream fcreate;
        fcreate.open(filename);
        fcreate << "Hello World";
        fcreate.close();
    }
    ~Scanner_fix()
    {
        remove(filename);
    }
};
TEST_F(Scanner_fix, validFile)
{
    char filename[] = "existing_file.txt";
    scanner sc;
    ASSERT_EQ(sc.getFileSize(filename), 11);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}