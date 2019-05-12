#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>
#include "catch.hpp"
#include "sound.h"

TEST_CASE("Constructor: 16 Bit Mono")
{
  Sound<std::int16_t> test(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  test.saveSound();

  std::ifstream loadFile ("16bitmono.raw", std::ios::in | std::ios::binary);
  loadFile.seekg(0, loadFile.end);
  int fileByteSize = loadFile.tellg();
  loadFile.seekg(0, loadFile.beg);
  std::int16_t buffer [fileByteSize/2];
  loadFile.read((char *)buffer, fileByteSize);
  loadFile.close();

  std::ifstream loadFile2 ("16bitmonotest.raw", std::ios::in | std::ios::binary);
  loadFile2.seekg(0, loadFile2.end);
  int fileByteSize2 = loadFile2.tellg();
  loadFile2.seekg(0, loadFile2.beg);
  std::int16_t buffer2 [fileByteSize2/2];
  loadFile2.read((char *)buffer2, fileByteSize2);
  loadFile2.close();

  REQUIRE(fileByteSize == fileByteSize2);

  for (int i = 0; i < fileByteSize/2; ++i)
  {
    //std::cout << i << ": Buffer 1: " << buffer[i] << " Buffer 2: " << buffer2[i] << "\n";
    REQUIRE(buffer[i] == buffer2[i]);
  }

}

TEST_CASE("Copy Semantics: 16 Bit Mono")
{
  Sound<std::int16_t> test(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);

  Sound<std::int16_t> test2(test);

  std::vector<std::int16_t> one = test.returnVector();
  std::vector<std::int16_t> two = test2.returnVector();

  REQUIRE(one == two);

  test = test2;
  std::vector<std::int16_t> three = test.returnVector();

  REQUIRE(two == three);

  Sound<std::int16_t> test3(Sound<std::int16_t>(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767));
  std::vector<int16_t> four = test3.returnVector();

  REQUIRE(four == one);

  test3 = Sound<std::int16_t>(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  std::vector<int16_t> five = test3.returnVector();

  REQUIRE(five == one);

}

TEST_CASE("+ operator: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  Sound<std::int16_t> test2(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);

  Sound<std::int16_t> result = test1 + test2;

  std::vector<int16_t> resultvec = result.returnVector();
  std::vector<int16_t> test1vec = test1.returnVector();
  std::vector<int16_t> test2vec = test2.returnVector();

  std::vector<int16_t> addResult;
  auto test1i = test1vec.begin();
  auto test2i = test2vec.begin();

  while (test1i != test1vec.end())
  {
    int op = (int)(*test1i) + (int)(*test2i);
    if (op > 32767)
    {
      op = 32767;
    }
    else if (op < -32768)
    {
      op = -32768;
    }
    int16_t final = op;
    addResult.push_back(op);

    ++test1i;
    ++test2i;
  }

  REQUIRE(addResult == resultvec);
}

TEST_CASE("| Operator: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  Sound<std::int16_t> test2(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);

  Sound<std::int16_t> result = test1 | test2;

  std::vector<int16_t> resultvec = result.returnVector();
  std::vector<int16_t> test1vec = test1.returnVector();
  std::vector<int16_t> test2vec = test2.returnVector();

  std::vector<int16_t> catResult;
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

TEST_CASE("* Operator: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  float volume = 1.5f;

  Sound<std::int16_t> result = test1 * volume;

  std::vector<int16_t> resultvec = result.returnVector();
  std::vector<int16_t> test1vec = test1.returnVector();

  std::vector<int16_t> volResult;
  auto test1i = test1vec.begin();
  while (test1i != test1vec.end())
  {
    float calc = (int)(*test1i) * volume;
    int op = (int)calc;
    if (op > 32767)
    {
      op = 32767;
    }
    else if (op < -32768)
    {
      op = -32768;
    }
    std::int16_t final = op;
    volResult.push_back(final);
    ++test1i;
  }

  REQUIRE(volResult == resultvec);

}

TEST_CASE("^ Operator: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  std::pair<int, int> cut = std::make_pair(20, 80);

  Sound<std::int16_t> result = test1 ^ cut;

  std::vector<int16_t> resultvec = result.returnVector();
  std::vector<int16_t> test1vec = test1.returnVector();

  std::vector<int16_t> cutResult;
  auto test1i = test1vec.begin() + cut.first;
  while (test1i != test1vec.begin() + cut.second)
  {
    cutResult.push_back(*test1i);
    ++test1i;
  }

  REQUIRE(cutResult == resultvec);
}

TEST_CASE("rangedAdd Method: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  Sound<std::int16_t> test2(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  int start = 20;
  int end = 80;

  Sound<std::int16_t> result = test1.rangedAdd(test2, 20, 80);

  std::vector<int16_t> resultvec = result.returnVector();
  std::vector<int16_t> test1vec = test1.returnVector();
  std::vector<int16_t> test2vec = test2.returnVector();

  std::vector<int16_t> radResult;
  auto test1i = test1vec.begin() + 20;
  auto test2i = test2vec.begin() + 20;

  while (test1i != test1vec.begin() + 80)
  {
    int op = (int)(*test1i) + (int)(*test2i);
    if (op > 32767)
    {
      op = 32767;
    }
    else if (op < -32768)
    {
      op = -32768;
    }
    int16_t final = op;
    radResult.push_back(final);

    ++test1i;
    ++test2i;
  }

  auto finali = test1vec.begin() + 20;
  auto radi = radResult.begin();
  while (finali != test1vec.begin() + 80)
  {
    *finali = *radi;
    ++finali;
    ++radi;
  }

  REQUIRE(test1vec == resultvec);

}

TEST_CASE("reverse Method: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);

  Sound<std::int16_t> result = test1.reverse();

  std::vector<int16_t> resultvec = result.returnVector();
  std::vector<int16_t> test1vec = test1.returnVector();

  std::vector<int16_t> revResult;
  auto test1i = test1vec.end() - 1;
  while (test1i != test1vec.begin() - 1)
  {
    revResult.push_back(*test1i);
    --test1i;
  }

  REQUIRE(revResult == resultvec);

}

TEST_CASE("rms Method: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);

  float result = test1.rms();

  std::vector<int16_t> test1vec = test1.returnVector();

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

TEST_CASE("normalisation Method: 16 Bit Mono")
{
  Sound<std::int16_t> test1(44100, "16bitmono.raw", "16bitmonotest.raw", 1, 2, 32767);
  float desiredRms = 1.5;
  float currentRms = test1.rms();

  Sound<std::int16_t> result = test1.normalisation(desiredRms);

  std::vector<int16_t> resultvec = result.returnVector();
  std::vector<int16_t> test1vec = test1.returnVector();

  std::vector<int16_t> norResult;
  auto test1i = test1vec.begin();

  while (test1i != test1vec.end())
  {
    float calc = (int)(*test1i) * (desiredRms / currentRms);
    int op = (int)(calc);
    if (op > 32767)
    {
      op = 32767;
    }
    else if (op < -32768)
    {
      op = -32768;
    }
    std::int16_t final = op;

    norResult.push_back(final);
    ++test1i;
  }

  REQUIRE(norResult == resultvec);
}
