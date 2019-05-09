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

template<typename T> class Sound
{
private:
    std::unique_ptr<std::vector<T>> soundstream;
    int sampleRate;
    int sizeOfIntN;
    int channels;
    int NumberOfSamples; //Need a function to calculate this
    int fileByteSize; //Need a function to calculate this
    std::string soundFile;
    std::string outputFile;
public:

  //Constructors
  Sound(std::vector<T> soundStream, int sampleRate, std::string soundFile, std::string outputFile, int channels, int sizeOfIntN): sampleRate(sampleRate), soundFile(soundFile), outputFile(outputFile), channels(channels), sizeOfIntN(sizeOfIntN)
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

  Sound(int sampleRate, std::string soundFile, std::string outputFile, int channels, int sizeOfIntN): sampleRate(sampleRate), soundFile(soundFile), outputFile(outputFile), channels(channels), sizeOfIntN(sizeOfIntN)
  {
    importSound();
  }

  Sound(const Sound & rhs): sampleRate(rhs.sampleRate), soundFile(rhs.soundFile), outputFile(rhs.outputFile), channels(rhs.channels), sizeOfIntN(rhs.sizeOfIntN), NumberOfSamples(rhs.NumberOfSamples), fileByteSize(rhs.fileByteSize)
  {
    std::unique_ptr<std::vector<T>> temp (new std::vector<T>);
    for (auto i = rhs.soundStream->begin(); i != rhs.soundStream->end(); ++i)
    {
      temp->push_back(*i);
    }
    soundstream = std::move(temp);
  }

  Sound(Sound && rhs): sampleRate(std::move(rhs.sampleRate)), soundFile(std::move(rhs.soundFile)), outputFile(std::move(rhs.outputFile)), channels(std::move(rhs.channels)), sizeOfIntN(std::move(sizeOfIntN)), NumberOfSamples(std::move(rhs.NumberOfSamples)), fileByteSize(std::move(rhs.fileByteSize))
  {
    soundstream = std::move(rhs.soundstream);
  }

  //File import and output
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

      loadFile.read((char *)buffer, NumberOfSamples);
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
    outFile.write((char *)buffer, NumberOfSamples);
    outFile.close();

  }

};




#endif
