#include <gtest/gtest.h>
#include <string>
#include <parent.h>


TEST(Lab1Test, CorrectSum) {
    std::streambuf* oldCoutBuf = std::cout.rdbuf();
    std::ostringstream outputStream;
    std::cout.rdbuf(outputStream.rdbuf());
    
    std::string testFile = "testfile.txt";

    std::string mockInput = "10 20 30 40 50 50 100";  
    std::istringstream inputStream(mockInput + '\n');
    std::streambuf* oldCinBuf = std::cin.rdbuf();
    std::cin.rdbuf(inputStream.rdbuf());

    ParentRoutine(testFile);

    std::cin.rdbuf(oldCinBuf);
    std::cout.rdbuf(oldCoutBuf);
    
    std::string output = outputStream.str();
    std::istringstream iss(output);
    int result;
    iss >> result;

    EXPECT_EQ(result, 300);
    
    std::remove(testFile.c_str());
}
