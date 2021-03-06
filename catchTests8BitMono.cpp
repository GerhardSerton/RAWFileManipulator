#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>
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

TEST_CASE("| Operator: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  Sound<std::int8_t> test2(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);

  Sound<std::int8_t> result = test1 | test2;

  std::vector<int8_t> resultvec = result.returnVector();
  std::vector<int8_t> test1vec = test1.returnVector();
  std::vector<int8_t> test2vec = test2.returnVector();

  std::vector<int8_t> catResult;
  auto test1i = test1vec.begin();
  auto test2i = test2vec.begin();

  while (test1i != test1vec.end())
  {
    catResult.push_back(*test1i);
    ++test1i;
  }

  while (test2i != test2vec.end())
  {
    catResult.push_back(*test2i);
    ++test2i;
  }

  REQUIRE(catResult == resultvec);
}

TEST_CASE("* Operator: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  float volume = 1.5f;

  Sound<std::int8_t> result = test1 * volume;

  std::vector<int8_t> resultvec = result.returnVector();
  std::vector<int8_t> test1vec = test1.returnVector();

  std::vector<int8_t> volResult;
  auto test1i = test1vec.begin();
  while (test1i != test1vec.end())
  {
    float calc = (int)(*test1i) * volume;
    int c = (int)calc;
    std::int8_t final = c;
    volResult.push_back(final);
    ++test1i;
  }

  REQUIRE(volResult == resultvec);

}

TEST_CASE("^ Operator: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  std::pair<float, float> cut = std::make_pair(1.0f, 3.0f);

  Sound<std::int8_t> result = test1 ^ cut;

  std::vector<int8_t> resultvec = result.returnVector();
  std::vector<int8_t> test1vec = test1.returnVector();

  std::vector<int8_t> cutResult;
  int start = (int)(cut.first * 44100.0f);
  int finish = (int)(cut.second * 44100.0f);
  auto test1i = test1vec.begin() + start;
  while (test1i != test1vec.begin() + finish)
  {
    cutResult.push_back(*test1i);
    ++test1i;
  }

  REQUIRE(cutResult == resultvec);
}

TEST_CASE("rangedAdd Method: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  Sound<std::int8_t> test2(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  int start = 20;
  int end = 80;

  Sound<std::int8_t> result = test1.rangedAdd(test2, 3.0f, 6.0f, 1.0f, 4.0f);

  std::vector<int8_t> resultvec = result.returnVector();
  std::vector<int8_t> test1vec = test1.returnVector();
  std::vector<int8_t> test2vec = test2.returnVector();

  int start1  = (int)(3.0f * 44100.0f);
  int finish1  = (int)(6.0f * 44100.0f);

  int start2  = (int)(1.0f * 44100.0f);
  int finish2  = (int)(4.0f * 44100.0f);

  std::vector<int8_t> radResult;
  auto test1i = test1vec.begin() + start1;
  auto test2i = test2vec.begin() + start2;

  while (test1i != test1vec.begin() + finish1)
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
    radResult.push_back(final);

    ++test1i;
    ++test2i;
  }
  /**
  auto finali = test1vec.begin() + 20;
  auto radi = radResult.begin();
  while (finali != test1vec.begin() + 80)
  {
    *finali = *radi;
    ++finali;
    ++radi;
  }

  REQUIRE(test1vec == resultvec);
  */

  REQUIRE(radResult == resultvec);

}

TEST_CASE("reverse Method: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);

  Sound<std::int8_t> result = test1.reverse();

  std::vector<int8_t> resultvec = result.returnVector();
  std::vector<int8_t> test1vec = test1.returnVector();

  std::vector<int8_t> revResult;
  auto test1i = test1vec.end() - 1;
  while (test1i != test1vec.begin() - 1)
  {
    revResult.push_back(*test1i);
    --test1i;
  }

  REQUIRE(revResult == resultvec);

}

TEST_CASE("rms Method: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);

  float result = test1.rms();

  std::vector<int8_t> test1vec = test1.returnVector();

  auto test1i = test1vec.begin();
  float total = 0.0f;
  while (test1i != test1vec.end())
  {
    total += (float)(*test1i) * (float)(*test1i);
    ++test1i;
  }
  float avg = total / (float)test1vec.size();
  float savg = sqrt(avg);

  REQUIRE(savg == result);
}

TEST_CASE("normalisation Method: 8 Bit Mono")
{
  Sound<std::int8_t> test1(44100, "8bitmono.raw", "8bitmonotest.raw", 1, 1, 127);
  float desiredRms = 1.5;
  float currentRms = test1.rms();

  Sound<std::int8_t> result = test1.normalisation(desiredRms);

  std::vector<int8_t> resultvec = result.returnVector();
  std::vector<int8_t> test1vec = test1.returnVector();

  std::vector<int8_t> norResult;
  auto test1i = test1vec.begin();

  while (test1i != test1vec.end())
  {
    float calc = (int)(*test1i) * (desiredRms / currentRms);
    int op = (int)(calc);
    if (op > 127)
    {
      op = 127;
    }
    else if (op < -128)
    {
      op = -128;
    }
    std::int8_t final = op;

    norResult.push_back(final);
    ++test1i;
  }

  REQUIRE(norResult == resultvec);
}
