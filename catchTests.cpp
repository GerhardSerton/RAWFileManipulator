#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include "catch.hpp"
#include "sound.h"

TEST_CASE("Constructor: 8 Bit Mono")
{
  Sound<std::int8_t> test(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  test.saveSound();

  std::ifstream loadFile ("8bitmono.raw", std::ios::in | std::ios::binary);
  loadFile.seekg(0, loadFile.end);
  int fileByteSize = loadFile.tellg();
  loadFile.seekg(0, loadFile.beg);
  std::int8_t buffer [fileByteSize];
  loadFile.read((char *)buffer, fileByteSize);
  loadFile.close();

  std::ifstream loadFile2 ("8bitmonotest.raw", std::ios::in | std::ios::binary);
  loadFile2.seekg(0, loadFile2.end);
  int fileByteSize2 = loadFile2.tellg();
  loadFile2.seekg(0, loadFile2.beg);
  std::int8_t buffer2 [fileByteSize2];
  loadFile2.read((char *)buffer2, fileByteSize2);
  loadFile2.close();

  REQUIRE(fileByteSize == fileByteSize2);

  for (int i = 0; i < fileByteSize; ++i)
  {
    //std::cout << i << ": Buffer 1: " << buffer[i] << " Buffer 2: " << buffer2[i] << "\n";
    REQUIRE(buffer[i] == buffer2[i]);
  }

}

TEST_CASE("Copy Semantics: 8 Bit Mono")
{
  Sound<std::int8_t> test(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);

  Sound<std::int8_t> test2(test);

  std::vector<std::int8_t> one = test.returnVector();
  std::vector<std::int8_t> two = test2.returnVector();

  REQUIRE(one == two);

  test = test2;
  std::vector<std::int8_t> three = test.returnVector();

  REQUIRE(two == three);

  Sound<std::int8_t> test3(Sound<std::int8_t>(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127));
  std::vector<int8_t> four = test3.returnVector();

  REQUIRE(four == one);

  test3 = Sound<std::int8_t>(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  std::vector<int8_t> five = test3.returnVector();

  REQUIRE(five == one);

}

TEST_CASE("+ operator: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  Sound<std::int8_t> test2(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);

  Sound<std::int8_t> result = test1 + test2;

  std::vector<int8_t> resultvec = result.returnVector();
  std::vector<int8_t> test1vec = test1.returnVector();
  std::vector<int8_t> test2vec = test2.returnVector();

  std::vector<int8_t> addResult;
  auto test1i = test1vec.begin();
  auto test2i = test2vec.begin();

  while (test1i != test1vec.end())
  {
    int op = (int)(*test1i) + (int)(*test2i);
    if (op > 127)
    {
      op = 127;
    }
    else if (op < -128)
    {
      op = -128;
    }
    int8_t final = op;
    addResult.push_back(op);

    ++test1i;
    ++test2i;
  }

  REQUIRE(addResult == resultvec);
}
