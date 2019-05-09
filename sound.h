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
    int NumberOfSamples;
    int fileByteSize;
    std::string soundFile;
    std::string outputFile;
public:
  Sound(std::vector<T> soundStream, int sampleRate, std::string soundFile, std::string outputFile): sampleRate(sampleRate), soundFile(soundFile), outputFile(outputFile)
  {
    std::unique_ptr<std::vector<T>> temp (new std::vector<T>);
    for (auto i = soundStream.begin(); i != soundStream.end(); ++i)
    {
      temp->push_back(*i);
    }
    soundstream = std::move(temp);
  }
};




#endif
