#include "../src/scanner.h"
#include <gtest/gtest.h>

TEST(ScannerTest, invalidFile)
{
    scanner sc;
    ASSERT_EQ(sc.getFileSize("non_existing_file.txt"), -1);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}