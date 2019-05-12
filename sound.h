#ifndef _SOUND_H_
#define _SOUND_H_

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <numeric>
#include <cmath>

template<typename T> class Sound
{
private:
    std::unique_ptr<std::vector<T>> soundstream;
    int sampleRate;
    int sizeOfIntN;
    int maximumSizeOfIntN;
    int channels;
    int NumberOfSamples; //Need a function to calculate this
    int fileByteSize; //Need a function to calculate this
    std::string soundFile;
    std::string outputFile;
public:

  //--------------------Constructors--------------------
  Sound(std::vector<T> soundStream, int sampleRate, std::string soundFile, std::string outputFile, int channels, int sizeOfIntN, int maximumSizeOfIntN): sampleRate(sampleRate), soundFile(soundFile), outputFile(outputFile), channels(channels), sizeOfIntN(sizeOfIntN), maximumSizeOfIntN(maximumSizeOfIntN)
  {
    std::unique_ptr<std::vector<T>> temp (new std::vector<T>);
    int count = 0;
    for (auto i = soundStream.begin(); i != soundStream.end(); ++i)
    {
      temp->push_back(*i);
      ++count;
    }
    NumberOfSamples = count;
    fileByteSize = NumberOfSamples * (sizeOfIntN * channels);
    soundstream = std::move(temp);
  }

  Sound(int sampleRate, std::string soundFile, std::string outputFile, int channels, int sizeOfIntN, int maximumSizeOfIntN): sampleRate(sampleRate), soundFile(soundFile), outputFile(outputFile), channels(channels), sizeOfIntN(sizeOfIntN), maximumSizeOfIntN(maximumSizeOfIntN)
  {
    importSound();
  }

  Sound(const Sound & rhs): sampleRate(rhs.sampleRate), soundFile(rhs.soundFile), outputFile(rhs.outputFile), channels(rhs.channels), sizeOfIntN(rhs.sizeOfIntN), NumberOfSamples(rhs.NumberOfSamples), fileByteSize(rhs.fileByteSize), maximumSizeOfIntN(rhs.maximumSizeOfIntN)
  {
    std::unique_ptr<std::vector<T>> temp (new std::vector<T>);
    for (auto i = rhs.soundstream->begin(); i != rhs.soundstream->end(); ++i)
    {
      temp->push_back(*i);
    }
    soundstream = std::move(temp);
  }

  Sound(Sound && rhs): sampleRate(std::move(rhs.sampleRate)), soundFile(std::move(rhs.soundFile)), outputFile(std::move(rhs.outputFile)), channels(std::move(rhs.channels)), sizeOfIntN(std::move(sizeOfIntN)), NumberOfSamples(std::move(rhs.NumberOfSamples)), fileByteSize(std::move(rhs.fileByteSize)), maximumSizeOfIntN(std::move(rhs.maximumSizeOfIntN))
  {
    soundstream = std::move(rhs.soundstream);
  }

  //--------------------File import and output--------------------
  void importSound()
  {
    std::ifstream loadFile (soundFile, std::ios::in | std::ios::binary);
    if (loadFile)
    {
      loadFile.seekg(0, loadFile.end);
      fileByteSize = loadFile.tellg();
      loadFile.seekg(0, loadFile.beg);
      //Remember: int8_t = 1 byte,  int 16_t = 2 bytes
      NumberOfSamples = fileByteSize / (sizeOfIntN * channels);
      T buffer [NumberOfSamples];

      loadFile.read((char *)buffer, fileByteSize);
      loadFile.close();

      std::unique_ptr<std::vector<T>> temp (new std::vector<T>);

      for (int i = 0; i < NumberOfSamples; ++i)
      {
        temp->push_back(buffer[i]);
      }

      soundstream = std::move(temp);

    }
    else
    {
      std::cout << "ERROR: FILE NOT FOUND\n";
    }
  }

  void saveSound()
  {
    std::ofstream outFile (outputFile, std::ios::out | std::ios::binary);
    T buffer [NumberOfSamples];
    for (int i = 0; i < NumberOfSamples; ++i)
    {
      buffer[i] = soundstream->at(i);
    }
    outFile.write((char *)buffer, fileByteSize);
    outFile.close();
  }

  //--------------------Operators--------------------

  Sound & operator = (const Sound & rhs)
  {
    sampleRate = rhs.sampleRate;
    soundFile = rhs.soundFile;
    outputFile = rhs.outputFile;
    channels = rhs.channels;
    sizeOfIntN = rhs.sizeOfIntN;
    NumberOfSamples = rhs.NumberOfSamples;
    fileByteSize = rhs.fileByteSize;
    maximumSizeOfIntN = rhs.maximumSizeOfIntN;

    std::unique_ptr<std::vector<T>> temp (new std::vector<T>);
    for (auto i = rhs.soundstream->begin(); i != rhs.soundstream->end(); ++i)
    {
      temp->push_back(*i);
    }
    soundstream = std::move(temp);

    return *this;
  }

  Sound & operator = (Sound && rhs)
  {
    sampleRate = std::move(rhs.sampleRate);
    soundFile = std::move(rhs.soundFile);
    outputFile = std::move(rhs.outputFile);
    channels = std::move(rhs.channels);
    sizeOfIntN = std::move(rhs.sizeOfIntN);
    NumberOfSamples = std::move(rhs.NumberOfSamples);
    fileByteSize = std::move(rhs.fileByteSize);
    maximumSizeOfIntN = std::move(rhs.maximumSizeOfIntN);
    soundstream = std::move(rhs.soundstream);

    return *this;
  }

  Sound operator + (Sound rhs) //add amplitudes
  {
    std::vector<T> temp;
    auto rhsi = rhs.soundstream->begin();
    auto thisi = soundstream->begin();
    while (thisi != soundstream->end())
    {
      int op = (int)(*rhsi) + (int)(*thisi);
      if (op > maximumSizeOfIntN)
      {
        op = maximumSizeOfIntN;
      }
      else if (op < (0 - maximumSizeOfIntN - 1))
      {
        op = (0 - maximumSizeOfIntN - 1);
      }
      T final = op;
      temp.push_back(op);

      ++thisi;
      ++rhsi;
    }

    return Sound<T>(temp, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
    }

    Sound operator | (Sound rhs) //concatenate
    {
      std::vector<T> result;
      std::copy(soundstream->begin(), soundstream->end(), back_inserter(result));
      std::copy(rhs.soundstream->begin(), rhs.soundstream->end(), back_inserter(result));

      return Sound<T>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
    }

    Sound operator * (float rhs) //volume factor
    {
      std::vector<T> result;
      int max = maximumSizeOfIntN;
      std::transform(soundstream->begin(), soundstream->end(), back_inserter(result), [rhs, max] (T x){
        int result = (int)((int)(x) * rhs);

        if (result > max)
        {
          result = max;
        }
        else if (result < (0 - max - 1))
        {
          result = (0 - max - 1);
        }

        T final = result;
        return final;
      });
      return Sound<T>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
    }

    Sound operator ^ (std::pair<int, int> rhs) //cut
    {
      std::vector<T> result;
      std::copy(soundstream->begin() + rhs.first, soundstream->begin() + rhs.second, back_inserter(result));

      return Sound<T>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
    }

    //--------------------Methods--------------------

    Sound rangedAdd(Sound rhs, int start, int end)
    {
      std::pair<int, int> cut = std::make_pair(start, end);
      Sound<T> cutResult = (rhs ^ cut) + (*this ^ cut);
      Sound<T> finalResult(*this);
      std::copy(cutResult.soundstream->begin(), cutResult.soundstream->end(), finalResult.soundstream->begin() + cut.first);

      return finalResult;

    }

    Sound reverse()
    {
      std::vector<T> result = *soundstream;
      std::reverse(result.begin(), result.end());

      return Sound<T>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);

    }
    std::vector<T> & returnVector()
    {
      return *soundstream;
    }

    float rms()
    {
      std::vector<float> squared;
      std::transform(soundstream->begin(), soundstream->end(), back_inserter(squared), [] (T x){
        float y = (float)(x);
        float result = (y * y);
        return result;
      });
      float result = std::accumulate(squared.begin(), squared.end(), 0.0f);
      result = result / (float)(NumberOfSamples);
      result = sqrt(result);
      return result;
    }

    Sound normalisation(float desiredRms)
    {
      float currentRms = rms();
      int max = maximumSizeOfIntN;
      std::vector<T> result;

      std::transform(soundstream->begin(), soundstream->end(), back_inserter(result), [desiredRms, currentRms, max] (T x) {
        float result = (float)(x) * (desiredRms / currentRms);
        int resultInt = (int)result;
        if (resultInt > max)
        {
          resultInt = max;
        }
        else if (resultInt < (0 - max - 1))
        {
          resultInt = (0 - max - 1);
        }
        T final = resultInt;

        return final;
      });

      return Sound<T>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);

    }


};

template<typename T> class Sound<std::pair<T, T>>
{
private:
  std::unique_ptr<std::vector<std::pair<T, T>>> soundstream;
  int sampleRate;
  int sizeOfIntN;
  int maximumSizeOfIntN;
  int channels;
  int NumberOfSamples; //Need a function to calculate this
  int fileByteSize; //Need a function to calculate this
  std::string soundFile;
  std::string outputFile;
public:

  //--------------------Constructors--------------------

  Sound(std::vector<std::pair<T, T>> soundStream, int sampleRate, std::string soundFile, std::string outputFile, int channels, int sizeOfIntN, int maximumSizeOfIntN): sampleRate(sampleRate), soundFile(soundFile), outputFile(outputFile), channels(channels), sizeOfIntN(sizeOfIntN), maximumSizeOfIntN(maximumSizeOfIntN)
  {
    std::unique_ptr<std::vector<std::pair<T, T>>> temp (new std::vector<std::pair<T, T>>);
    int count = 0;
    for (auto i = soundStream.begin(); i != soundStream.end(); ++i)
    {
      temp->push_back(*i);
      ++count;
    }
    NumberOfSamples = count;
    fileByteSize = NumberOfSamples * (sizeOfIntN * channels);
    soundstream = std::move(temp);
  }

  Sound(int sampleRate, std::string soundFile, std::string outputFile, int channels, int sizeOfIntN, int maximumSizeOfIntN): sampleRate(sampleRate), soundFile(soundFile), outputFile(outputFile), channels(channels), sizeOfIntN(sizeOfIntN), maximumSizeOfIntN(maximumSizeOfIntN)
  {
    importSound();
  }

  Sound(const Sound & rhs): sampleRate(rhs.sampleRate), soundFile(rhs.soundFile), outputFile(rhs.outputFile), channels(rhs.channels), sizeOfIntN(rhs.sizeOfIntN), NumberOfSamples(rhs.NumberOfSamples), fileByteSize(rhs.fileByteSize), maximumSizeOfIntN(rhs.maximumSizeOfIntN)
  {
    std::unique_ptr<std::vector<std::pair<T, T>>> temp (new std::vector<std::pair<T, T>>);
    for (auto i = rhs.soundstream->begin(); i != rhs.soundstream->end(); ++i)
    {
      temp->push_back(*i);
    }
    soundstream = std::move(temp);
  }

  Sound(Sound && rhs): sampleRate(std::move(rhs.sampleRate)), soundFile(std::move(rhs.soundFile)), outputFile(std::move(rhs.outputFile)), channels(std::move(rhs.channels)), sizeOfIntN(std::move(sizeOfIntN)), NumberOfSamples(std::move(rhs.NumberOfSamples)), fileByteSize(std::move(rhs.fileByteSize)), maximumSizeOfIntN(std::move(rhs.maximumSizeOfIntN))
  {
    soundstream = std::move(rhs.soundstream);
  }

  //--------------------File import and output--------------------
  void importSound()
  {
    std::ifstream loadFile (soundFile, std::ios::in | std::ios::binary);
    if (loadFile)
    {
      loadFile.seekg(0, loadFile.end);
      fileByteSize = loadFile.tellg();
      loadFile.seekg(0, loadFile.beg);
      //Remember: int8_t = 1 byte,  int 16_t = 2 bytes
      NumberOfSamples = fileByteSize / (sizeOfIntN * channels);
      T buffer [fileByteSize/sizeOfIntN];

      loadFile.read((char *)buffer, fileByteSize);
      loadFile.close();

      std::unique_ptr<std::vector<std::pair<T, T>>> temp (new std::vector<std::pair<T, T>>);

      for (int i = 0; i < fileByteSize/sizeOfIntN; i = i+2)
      {
        std::pair<T, T> tpair = std::make_pair(buffer[i], buffer[i+1]);
        temp->push_back(tpair);
      }

      soundstream = std::move(temp);

    }
    else
    {
      std::cout << "ERROR: FILE NOT FOUND\n";
    }
  }

  void saveSound()
  {
    std::ofstream outFile (outputFile, std::ios::out | std::ios::binary);
    T buffer [fileByteSize/sizeOfIntN];
    int bufferPosition = 0;
    for (auto i = soundstream->begin(); i != soundstream->end(); ++i)
    {
      std::pair<T, T> t = *i;
      buffer[bufferPosition] = t.first;
      buffer[bufferPosition + 1] = t.second;
      bufferPosition = bufferPosition + 2;
    }
    outFile.write((char *)buffer, fileByteSize);
    outFile.close();
  }

  //--------------------Operators--------------------

  Sound & operator = (const Sound & rhs)
  {
    sampleRate = rhs.sampleRate;
    soundFile = rhs.soundFile;
    outputFile = rhs.outputFile;
    channels = rhs.channels;
    sizeOfIntN = rhs.sizeOfIntN;
    NumberOfSamples = rhs.NumberOfSamples;
    fileByteSize = rhs.fileByteSize;
    maximumSizeOfIntN = rhs.maximumSizeOfIntN;

    std::unique_ptr<std::vector<std::pair<T, T>>> temp (new std::vector<std::pair<T, T>>);
    for (auto i = rhs.soundstream->begin(); i != rhs.soundstream->end(); ++i)
    {
      temp->push_back(*i);
    }
    soundstream = std::move(temp);

    return *this;
  }

  Sound & operator = (Sound && rhs)
  {
    sampleRate = std::move(rhs.sampleRate);
    soundFile = std::move(rhs.soundFile);
    outputFile = std::move(rhs.outputFile);
    channels = std::move(rhs.channels);
    sizeOfIntN = std::move(rhs.sizeOfIntN);
    NumberOfSamples = std::move(rhs.NumberOfSamples);
    fileByteSize = std::move(rhs.fileByteSize);
    maximumSizeOfIntN = std::move(rhs.maximumSizeOfIntN);
    soundstream = std::move(rhs.soundstream);

    return *this;
  }

  Sound operator + (Sound rhs) //add amplitudes
  {
    std::vector<std::pair<T, T>> temp;
    auto rhsi = rhs.soundstream->begin();
    auto thisi = soundstream->begin();
    while (thisi != soundstream->end())
    {
      std::pair<T, T> rhsp = *rhsi;
      std::pair<T, T> thisp = *thisi;
      int op1 = (int)(rhsp.first) + (int)(thisp.first);
      if (op1 > maximumSizeOfIntN)
      {
        op1 = maximumSizeOfIntN;
      }
      else if (op1 < (0 - maximumSizeOfIntN - 1))
      {
        op1 = (0 - maximumSizeOfIntN - 1);
      }

      int op2 = (int)(rhsp.second) + (int)(thisp.second);
      if (op2 > maximumSizeOfIntN)
      {
        op2 = maximumSizeOfIntN;
      }
      else if (op2 < (0 - maximumSizeOfIntN - 1))
      {
        op2 = (0 - maximumSizeOfIntN - 1);
      }

      T left = op1;
      T right = op2;
      std::pair<T, T> final = std::make_pair(left, right);
      temp.push_back(final);

      ++thisi;
      ++rhsi;
    }

    return Sound<std::pair<T, T>>(temp, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
  }

  Sound operator | (Sound rhs) //concatenate
  {
    std::vector<std::pair<T, T>> result;
    std::copy(soundstream->begin(), soundstream->end(), back_inserter(result));
    std::copy(rhs.soundstream->begin(), rhs.soundstream->end(), back_inserter(result));

    return Sound<std::pair<T, T>>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
  }

  Sound operator * (std::pair<float, float> rhs) //volume factor
  {
    std::vector<std::pair<T, T>> result;
    int max = maximumSizeOfIntN;
    std::transform(soundstream->begin(), soundstream->end(), back_inserter(result), [rhs, max] (std::pair<T, T> x) {
      int left = (int)((int)(x.first) * rhs.first);
      int right = (int)((int)(x.second) * rhs.second);

      if (left > max)
      {
        left = max;
      }
      else if (left < (0 - max - 1))
      {
        left = (0 - max - 1);
      }

      if (right > max)
      {
        right = max;
      }
      else if (right < (0 - max - 1))
      {
        right = (0 - max - 1);
      }
      T l = left;
      T r = right;
      std::pair<T, T> final = std::make_pair(l, r);
      return final;
    });
    return Sound<std::pair<T, T>>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
  }

  Sound operator ^ (std::pair<int, int> rhs) //cut
  {
    std::vector<std::pair<T, T>> result;
    std::copy(soundstream->begin() + rhs.first, soundstream->begin() + rhs.second, back_inserter(result));

    return Sound<std::pair<T, T>>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);
  }

  //--------------------Methods--------------------

  std::vector<std::pair<T, T>> & returnVector()
  {
    return *soundstream;
  }

  Sound rangedAdd(Sound rhs, int start, int end)
  {
    std::pair<int, int> cut = std::make_pair(start, end);
    Sound<std::pair<T, T>> cutResult = (rhs ^ cut) + (*this ^ cut);
    Sound<std::pair<T, T>> finalResult(*this);
    std::copy(cutResult.soundstream->begin(), cutResult.soundstream->end(), finalResult.soundstream->begin() + cut.first);

    return finalResult;
  }

  Sound reverse()
  {
    std::vector<std::pair<T, T>> result = *soundstream;
    std::reverse(result.begin(), result.end());

    return Sound<std::pair<T, T>>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);

  }

  std::pair<float, float> rms()
  {
    std::vector<float> leftsquared;
    std::vector<float> rightsquared;

    std::transform(soundstream->begin(), soundstream->end(), back_inserter(leftsquared), [] (std::pair<T, T> x){
      float left = (float)(x.first);
      left = left * left;

      return left;
    });

    std::transform(soundstream->begin(), soundstream->end(), back_inserter(rightsquared), [] (std::pair<T, T> x){
      float right = (float)(x.second);
      right = right * right;

      return right;
    });

    float resultLeft = std::accumulate(leftsquared.begin(), leftsquared.end(), 0.0f);
    float resultRight = std::accumulate(rightsquared.begin(), rightsquared.end(), 0.0f);
    resultLeft = sqrt(resultLeft / (float)(NumberOfSamples));
    resultRight = sqrt(resultRight / (float)(NumberOfSamples));
    return std::make_pair(resultLeft, resultRight);
  }

  Sound normalisation(std::pair<float, float> desiredRms)
  {
    std::pair<float, float> currentRms = rms();
    int max = maximumSizeOfIntN;
    std::vector<std::pair<T, T>> result;

    std::transform(soundstream->begin(), soundstream->end(), back_inserter(result), [desiredRms, currentRms, max] (std::pair<T, T> x) {
      float leftFloat = (float)(x.first) * (desiredRms.first / currentRms.first);
      float rightFloat = (float)(x.second) * (desiredRms.second / currentRms.second);

      int left = (int)(leftFloat);
      int right = (int)(rightFloat);

      if (left > max)
      {
        left = max;
      }
      else if (left < (0 - max - 1))
      {
        left = (0 - max - 1);
      }

      if (right > max)
      {
        right = max;
      }
      else if (right < (0 - max - 1))
      {
        right = (0 - max - 1);
      }

      T l = left;
      T r = right;

      std::pair<T, T> final = std::make_pair(l, r);

      return final;
    });

    return Sound<std::pair<T, T>>(result, sampleRate, soundFile, outputFile, channels, sizeOfIntN, maximumSizeOfIntN);

  }
};




#endif
