#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>
#include <utility>
#include "catch.hpp"
#include "sound.h"

TEST_CASE("Constructor: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);

  test.saveSound();

  std::ifstream loadFile ("8bitstereo.raw", std::ios::in | std::ios::binary);
  loadFile.seekg(0, loadFile.end);
  int fileByteSize = loadFile.tellg();
  loadFile.seekg(0, loadFile.beg);
  std::int8_t buffer [fileByteSize];
  loadFile.read((char *)buffer, fileByteSize);
  loadFile.close();

  std::ifstream loadFile2 ("8bitstereotest.raw", std::ios::in | std::ios::binary);
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
    //std::cout << "fileByteSize: " << fileByteSize << "\n";
    REQUIRE(buffer[i] == buffer2[i]);
  }

}

TEST_CASE("Copy Semantics: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);

  Sound<std::pair<std::int8_t, std::int8_t>> test2(test);

  std::vector<std::pair<std::int8_t, std::int8_t>> one = test.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> two = test2.returnVector();

  REQUIRE(one == two);

  test = test2;
  std::vector<std::pair<std::int8_t, std::int8_t>> three = test.returnVector();

  REQUIRE(two == three);

  Sound<std::pair<std::int8_t, std::int8_t>> test3(Sound<std::pair<std::int8_t, std::int8_t>>(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127));
  std::vector<std::pair<std::int8_t, std::int8_t>> four = test3.returnVector();

  REQUIRE(four == one);

  test3 = Sound<std::pair<std::int8_t, std::int8_t>>(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  std::vector<std::pair<std::int8_t, std::int8_t>> five = test3.returnVector();

  REQUIRE(five == one);
}

TEST_CASE("+ operator: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  Sound<std::pair<std::int8_t, std::int8_t>> test2(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);

  Sound<std::pair<std::int8_t, std::int8_t>> result = test1 + test2;

  std::vector<std::pair<std::int8_t, std::int8_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int8_t, std::int8_t>> addResult;
  auto test1i = test1vec.begin();

  while (test1i != test1vec.end())
  {
    std::pair<std::int8_t, std::int8_t> t1p = *test1i;
    int leftInt = (int)t1p.first * 2;
    int rightInt = (int)t1p.second * 2;
    if (leftInt > 127)
    {
      leftInt = 127;
    }
    else if (leftInt < -128)
    {
      leftInt < -128;
    }

    if (rightInt > 127)
    {
      rightInt = 127;
    }
    else if (rightInt < -128)
    {
      rightInt < -128;
    }
    std::int8_t left = leftInt;
    std::int8_t right = rightInt;
    std::pair<std::int8_t, std::int8_t> rp = std::make_pair(left, right);

    addResult.push_back(rp);

    ++test1i;
  }

  REQUIRE(addResult == resultvec);
}

TEST_CASE("| Operator: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  Sound<std::pair<std::int8_t, std::int8_t>> test2(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);

  Sound<std::pair<std::int8_t, std::int8_t>> result = test1 | test2;

  std::vector<std::pair<std::int8_t, std::int8_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test2vec = test2.returnVector();

  std::vector<std::pair<std::int8_t, std::int8_t>> catResult;
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

TEST_CASE("* Operator: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  std::pair<float, float> volume = std::make_pair(1.5f, 3.0f);

  Sound<std::pair<std::int8_t, std::int8_t>> result = test1 * volume;

  std::vector<std::pair<std::int8_t, std::int8_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int8_t, std::int8_t>> volResult;
  auto test1i = test1vec.begin();
  while (test1i != test1vec.end())
  {
    std::pair<std::int8_t, std::int8_t> t1p = *test1i;

    float lf = (float)(t1p.first) * volume.first;
    float rf = (float)(t1p.second) * volume.second;

    if (lf > 127.0f)
    {
      lf = 127.0f;
    }
    else if (lf < -128.0f)
    {
      lf = -128.0f;
    }

    if (rf > 127.0f)
    {
      rf = 127.0f;
    }
    else if (rf < -128.0f)
    {
      rf = -128.0f;
    }

    std::int8_t left = (int)lf;
    std::int8_t right = (int)rf;
    std::pair<std::int8_t, std::int8_t> presult = std::make_pair(left, right);
    volResult.push_back(presult);
    ++test1i;
  }

  REQUIRE(volResult == resultvec);


}

TEST_CASE("^ Operator: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  std::pair<int, int> cut = std::make_pair(20, 80);

  Sound<std::pair<std::int8_t, std::int8_t>> result = test1 ^ cut;

  std::vector<std::pair<std::int8_t, std::int8_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int8_t, std::int8_t>> cutResult;
  auto test1i = test1vec.begin() + cut.first;
  while (test1i != test1vec.begin() + cut.second)
  {
    cutResult.push_back(*test1i);
    ++test1i;
  }

  REQUIRE(cutResult == resultvec);
}

TEST_CASE("rangedAdd Method: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  Sound<std::pair<std::int8_t, std::int8_t>> test2(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  int start = 20;
  int end = 80;

  Sound<std::pair<std::int8_t, std::int8_t>> result = test1.rangedAdd(test2, 20, 80, 40, 100);
  std::vector<std::pair<std::int8_t, std::int8_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test2vec = test2.returnVector();

  std::vector<std::pair<std::int8_t, std::int8_t>> radResult;
  auto test1i = test1vec.begin() + 20;
  auto test2i = test2vec.begin() + 40;

  while (test1i != test1vec.begin() + 80)
  {
    std::pair<std::int8_t, std::int8_t> t1p = *test1i;
    std::pair<std::int8_t, std::int8_t> t2p = *test2i;
    int leftInt = (int)(t1p.first) + (int)(t2p.first);
    int rightInt = (int)(t1p.second) + (int)(t2p.second);
    if (leftInt > 127)
    {
      leftInt = 127;
    }
    else if (leftInt < -128)
    {
      leftInt < -128;
    }

    if (rightInt > 127)
    {
      rightInt = 127;
    }
    else if (rightInt < -128)
    {
      rightInt < -128;
    }

    std::int8_t left = leftInt;
    std::int8_t right = rightInt;

    std::pair<std::int8_t, std::int8_t> resultpair = std::make_pair(left, right);
    radResult.push_back(resultpair);

    ++test1i;
    ++test2i;
  }
  /**
  auto finali = test1vec.begin() + 20;
  auto radi = radResult.begin();
  while (radi != radResult.end())
  {
    *finali = *radi;
    ++finali;
    ++radi;
  }

  REQUIRE(test1vec == resultvec);
  */
  REQUIRE(radResult == resultvec);
}

TEST_CASE("reverse Method: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);

  Sound<std::pair<std::int8_t, std::int8_t>> result = test1.reverse();

  std::vector<std::pair<std::int8_t, std::int8_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int8_t, std::int8_t>> revResult;
  auto test1i = test1vec.end() - 1;
  while (test1i != test1vec.begin() - 1)
  {
    revResult.push_back(*test1i);
    --test1i;
  }

  REQUIRE(revResult == resultvec);
}

TEST_CASE("rms Method: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);

  std::pair<float, float> result = test1.rms();

  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();

  auto test1i = test1vec.begin();
  float lefttotal = 0.0f;
  float righttotal = 0.0f;
  while (test1i != test1vec.end())
  {
    std::pair<std::int8_t, std::int8_t> p = *test1i;
    lefttotal += (float)(p.first) * (float)(p.first);
    righttotal += (float)(p.second) * (float)(p.second);
    ++test1i;
  }
  float savgLeft = sqrt(lefttotal / (float)(test1vec.size()));
  float savgRight = sqrt(righttotal / (float)(test1vec.size()));

  REQUIRE(savgLeft == result.first);
  REQUIRE(savgRight == result.second);
}

TEST_CASE("normalisation Method: 8 Bit Stereo")
{
  Sound<std::pair<std::int8_t, std::int8_t>> test1(44100, "8bitstereo.raw", "8bitstereotest.raw", 2, 1, 127);
  std::pair<float, float> desiredRms = std::make_pair(1.5f, 3.0f);
  std::pair<float, float> currentRms = test1.rms();

  Sound<std::pair<std::int8_t, std::int8_t>> result = test1.normalisation(desiredRms);

  std::vector<std::pair<std::int8_t, std::int8_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int8_t, std::int8_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int8_t, std::int8_t>> norResult;
  auto test1i = test1vec.begin();

  while (test1i != test1vec.end())
  {
    std::pair<std::int8_t, std::int8_t> p = *test1i;
    float leftFloat = (float)(p.first) * (desiredRms.first / currentRms.first);
    float rightFloat = (float)(p.second) * (desiredRms.second / currentRms.second);

    int left = (int)(leftFloat);
    int right = (int)(rightFloat);

    if (left > 127)
    {
      left = 127;
    }
    else if (left < -128)
    {
      left = -128;
    }

    if (right > 127)
    {
      right = 127;
    }
    else if (right < -128)
    {
      right = -128;
    }

    std::int8_t l = left;
    std::int8_t r = right;

    std::pair<int8_t, int8_t> final = std::make_pair(l, r);

    norResult.push_back(final);

    ++test1i;
  }

  REQUIRE(norResult == resultvec);

}
