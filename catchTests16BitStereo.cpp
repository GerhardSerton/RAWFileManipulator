#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>
#include <utility>
#include "catch.hpp"
#include "sound.h"
TEST_CASE("Constructor: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);

  test.saveSound();

  std::ifstream loadFile ("16bitstereo.raw", std::ios::in | std::ios::binary);
  loadFile.seekg(0, loadFile.end);
  int fileByteSize = loadFile.tellg();
  loadFile.seekg(0, loadFile.beg);
  std::int16_t buffer [fileByteSize/2];
  loadFile.read((char *)buffer, fileByteSize);
  loadFile.close();

  std::ifstream loadFile2 ("16bitstereotest.raw", std::ios::in | std::ios::binary);
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
    //std::cout << "fileByteSize: " << fileByteSize << "\n";
    REQUIRE(buffer[i] == buffer2[i]);
  }

}

TEST_CASE("Copy Semantics: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);

  Sound<std::pair<std::int16_t, std::int16_t>> test2(test);

  std::vector<std::pair<std::int16_t, std::int16_t>> one = test.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> two = test2.returnVector();

  REQUIRE(one == two);

  test = test2;
  std::vector<std::pair<std::int16_t, std::int16_t>> three = test.returnVector();

  REQUIRE(two == three);

  Sound<std::pair<std::int16_t, std::int16_t>> test3(Sound<std::pair<std::int16_t, std::int16_t>>(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767));
  std::vector<std::pair<std::int16_t, std::int16_t>> four = test3.returnVector();

  REQUIRE(four == one);

  test3 = Sound<std::pair<std::int16_t, std::int16_t>>(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  std::vector<std::pair<std::int16_t, std::int16_t>> five = test3.returnVector();

  REQUIRE(five == one);
}

TEST_CASE("+ operator: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  Sound<std::pair<std::int16_t, std::int16_t>> test2(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);

  Sound<std::pair<std::int16_t, std::int16_t>> result = test1 + test2;

  std::vector<std::pair<std::int16_t, std::int16_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int16_t, std::int16_t>> addResult;
  auto test1i = test1vec.begin();

  while (test1i != test1vec.end())
  {
    std::pair<std::int16_t, std::int16_t> t1p = *test1i;
    int leftInt = (int)t1p.first * 2;
    int rightInt = (int)t1p.second * 2;
    if (leftInt > 32767)
    {
      leftInt = 32767;
    }
    else if (leftInt < -32768)
    {
      leftInt < -32768;
    }

    if (rightInt > 32767)
    {
      rightInt = 32767;
    }
    else if (rightInt < -32768)
    {
      rightInt < -32768;
    }
    std::int16_t left = leftInt;
    std::int16_t right = rightInt;
    std::pair<std::int16_t, std::int16_t> rp = std::make_pair(left, right);

    addResult.push_back(rp);

    ++test1i;
  }

  REQUIRE(addResult == resultvec);
}

TEST_CASE("| Operator: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  Sound<std::pair<std::int16_t, std::int16_t>> test2(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);

  Sound<std::pair<std::int16_t, std::int16_t>> result = test1 | test2;

  std::vector<std::pair<std::int16_t, std::int16_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test2vec = test2.returnVector();

  std::vector<std::pair<std::int16_t, std::int16_t>> catResult;
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

TEST_CASE("* Operator: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  std::pair<float, float> volume = std::make_pair(1.5f, 3.0f);

  Sound<std::pair<std::int16_t, std::int16_t>> result = test1 * volume;

  std::vector<std::pair<std::int16_t, std::int16_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int16_t, std::int16_t>> volResult;
  auto test1i = test1vec.begin();
  while (test1i != test1vec.end())
  {
    std::pair<std::int16_t, std::int16_t> t1p = *test1i;

    float lf = (float)(t1p.first) * volume.first;
    float rf = (float)(t1p.second) * volume.second;

    if (lf > 32767.0f)
    {
      lf = 32767.0f;
    }
    else if (lf < -32768.0f)
    {
      lf = -32768.0f;
    }

    if (rf > 32767.0f)
    {
      rf = 32767.0f;
    }
    else if (rf < -32768.0f)
    {
      rf = -32768.0f;
    }

    std::int16_t left = (int)lf;
    std::int16_t right = (int)rf;
    std::pair<std::int16_t, std::int16_t> presult = std::make_pair(left, right);
    volResult.push_back(presult);
    ++test1i;
  }

  REQUIRE(volResult == resultvec);


}

TEST_CASE("^ Operator: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  std::pair<float, float> cut = std::make_pair(1.0f, 3.0f);

  Sound<std::pair<std::int16_t, std::int16_t>> result = test1 ^ cut;

  std::vector<std::pair<std::int16_t, std::int16_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int16_t, std::int16_t>> cutResult;
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

TEST_CASE("rangedAdd Method: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  Sound<std::pair<std::int16_t, std::int16_t>> test2(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  int start = 20;
  int end = 80;

  Sound<std::pair<std::int16_t, std::int16_t>> result = test1.rangedAdd(test2, 3.0f, 6.0f, 1.0f, 4.0f);
  std::vector<std::pair<std::int16_t, std::int16_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test2vec = test2.returnVector();

  int start1  = (int)(3.0f * 44100.0f);
  int finish1  = (int)(6.0f * 44100.0f);

  int start2  = (int)(1.0f * 44100.0f);
  int finish2  = (int)(4.0f * 44100.0f);

  std::vector<std::pair<std::int16_t, std::int16_t>> radResult;
  auto test1i = test1vec.begin() + start1;
  auto test2i = test2vec.begin() + start2;

  while (test1i != test1vec.begin() + finish1)
  {
    std::pair<std::int16_t, std::int16_t> t1p = *test1i;
    std::pair<std::int16_t, std::int16_t> t2p = *test2i;
    int leftInt = (int)(t1p.first) + (int)(t2p.first);
    int rightInt = (int)(t1p.second) + (int)(t2p.second);
    if (leftInt > 32767)
    {
      leftInt = 32767;
    }
    else if (leftInt < -32768)
    {
      leftInt < -32768;
    }

    if (rightInt > 32767)
    {
      rightInt = 32767;
    }
    else if (rightInt < -32768)
    {
      rightInt < -32768;
    }

    std::int16_t left = leftInt;
    std::int16_t right = rightInt;

    std::pair<std::int16_t, std::int16_t> resultpair = std::make_pair(left, right);
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

TEST_CASE("reverse Method: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);

  Sound<std::pair<std::int16_t, std::int16_t>> result = test1.reverse();

  std::vector<std::pair<std::int16_t, std::int16_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int16_t, std::int16_t>> revResult;
  auto test1i = test1vec.end() - 1;
  while (test1i != test1vec.begin() - 1)
  {
    revResult.push_back(*test1i);
    --test1i;
  }

  REQUIRE(revResult == resultvec);
}

TEST_CASE("rms Method: 16 Bit Stereo")
{
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);

  std::pair<float, float> result = test1.rms();

  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();

  auto test1i = test1vec.begin();
  float lefttotal = 0.0f;
  float righttotal = 0.0f;
  while (test1i != test1vec.end())
  {
    std::pair<std::int16_t, std::int16_t> p = *test1i;
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
  Sound<std::pair<std::int16_t, std::int16_t>> test1(44100, "16bitstereo.raw", "16bitstereotest.raw", 2, 2, 32767);
  std::pair<float, float> desiredRms = std::make_pair(1.5f, 3.0f);
  std::pair<float, float> currentRms = test1.rms();

  Sound<std::pair<std::int16_t, std::int16_t>> result = test1.normalisation(desiredRms);

  std::vector<std::pair<std::int16_t, std::int16_t>> resultvec = result.returnVector();
  std::vector<std::pair<std::int16_t, std::int16_t>> test1vec = test1.returnVector();

  std::vector<std::pair<std::int16_t, std::int16_t>> norResult;
  auto test1i = test1vec.begin();

  while (test1i != test1vec.end())
  {
    std::pair<std::int16_t, std::int16_t> p = *test1i;
    float leftFloat = (float)(p.first) * (desiredRms.first / currentRms.first);
    float rightFloat = (float)(p.second) * (desiredRms.second / currentRms.second);

    int left = (int)(leftFloat);
    int right = (int)(rightFloat);

    if (left > 32767)
    {
      left = 32767;
    }
    else if (left < -32768)
    {
      left = -32768;
    }

    if (right > 32767)
    {
      right = 32767;
    }
    else if (right < -32768)
    {
      right = -32768;
    }

    std::int16_t l = left;
    std::int16_t r = right;

    std::pair<int16_t, int16_t> final = std::make_pair(l, r);

    norResult.push_back(final);

    ++test1i;
  }

  REQUIRE(norResult == resultvec);

}
