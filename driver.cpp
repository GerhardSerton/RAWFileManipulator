#include <string>
#include <cmath>
#include <bits/stdc++.h>
#include "sound.h"

using namespace std;
int main(int argc, char * argv[])
{
	int sampleRate = stoi(argv[2]);
	int bitCount = stoi(argv[4]) / 8;
	string bitCountString = argv[4];
	int highValue = ((int)(pow(2.0,(double)bitCount)) / 2) - 1;
	int channels = stoi(argv[6]);
	string channelsString = "MONO";
	if (channels == 2)
	{
		channelsString = "STEREO";
	}
	string outName = "out";
	string argument = argv[7];
	if (argument == "-o")
	{
		outName = argv[8];
		argument = argv[9];
	}
	string outNameFinal;
	outNameFinal = outName + "_" + to_string(sampleRate) + "_" + bitCountString + "_" + channelsString + ".raw";
	cout << "Outputting to: " << outNameFinal << "\n";
	cout << "Argument: " << argument << "\n";


	if (argument == "-add")
	{
		string soundFile1 = argv[10];
		string soundFile2 = argv[11];
		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);
				Sound<int8_t> sound2(sampleRate, soundFile2, outNameFinal, 1, 1, 127);

				Sound<int8_t> sound3 = sound1 + sound2;
				sound3.saveSound();
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);
				Sound<int16_t> sound2(sampleRate, soundFile2, outNameFinal, 1, 2, 32767);

				Sound<int16_t> sound3 = sound1 + sound2;
				sound3.saveSound();
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);
				Sound<pair<int8_t, int8_t>> sound2(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				Sound<pair<int8_t, int8_t>> sound3 = sound1 + sound2;
				sound3.saveSound();
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);
				Sound<pair<int16_t, int16_t>> sound2(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				Sound<pair<int16_t, int16_t>> sound3 = sound1 + sound2;
				sound3.saveSound();
			}
		}
	}

	else if (argument == "-cut")
	{
		string soundFile1 = argv[12];
		pair<int, int> cut = make_pair(stof(argv[10]), stof(argv[11]));
		cout << stof(argv[10]) << " " << stof(argv[11]) << "\n";
		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);

				Sound<int8_t> sound3 = sound1 ^ cut;
				sound3.saveSound();
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);

				Sound<int16_t> sound3 = sound1 ^ cut;
				sound3.saveSound();
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				Sound<pair<int8_t, int8_t>> sound3 = sound1 ^ cut;
				sound3.saveSound();
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				Sound<pair<int16_t, int16_t>> sound3 = sound1 ^ cut;
				sound3.saveSound();
			}
		}
	}
	else if (argument == "-radd") //make it work for 2 seperate cuts.
	{
		string soundFile1 = argv[14];
		string soundFile2 = argv[15];
		int start1 = stof(argv[10]);
		int end1 = stof(argv[11]);
		int start2 = stof(argv[12]);
		int end2 = stof(argv[13]);

		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);
				Sound<int8_t> sound2(sampleRate, soundFile2, outNameFinal, 1, 1, 127);

				Sound<int8_t> sound3 = sound1.rangedAdd(sound2, start1, end1, start2, end2);
				sound3.saveSound();
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);
				Sound<int16_t> sound2(sampleRate, soundFile2, outNameFinal, 1, 2, 32767);

				Sound<int16_t> sound3 = sound1.rangedAdd(sound2, start1, end1, start2, end2);
				sound3.saveSound();
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);
				Sound<pair<int8_t, int8_t>> sound2(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				Sound<pair<int8_t, int8_t>> sound3 = sound1.rangedAdd(sound2, start1, end1, start2, end2);
				sound3.saveSound();
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);
				Sound<pair<int16_t, int16_t>> sound2(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				Sound<pair<int16_t, int16_t>> sound3 = sound1.rangedAdd(sound2, start1, end1, start2, end2);
				sound3.saveSound();
			}
		}
	}
	else if (argument == "-cat")
	{
		string soundFile1 = argv[10];
		string soundFile2 = argv[11];

		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);
				Sound<int8_t> sound2(sampleRate, soundFile2, outNameFinal, 1, 1, 127);

				Sound<int8_t> sound3 = sound1 | sound2;
				sound3.saveSound();
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);
				Sound<int16_t> sound2(sampleRate, soundFile2, outNameFinal, 1, 2, 32767);

				Sound<int16_t> sound3 = sound1 | sound2;
				sound3.saveSound();
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);
				Sound<pair<int8_t, int8_t>> sound2(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				Sound<pair<int8_t, int8_t>> sound3 = sound1 | sound2;
				sound3.saveSound();
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);
				Sound<pair<int16_t, int16_t>> sound2(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				Sound<pair<int16_t, int16_t>> sound3 = sound1 | sound2;
				sound3.saveSound();
			}
		}
	}
	else if (argument == "-v")
	{
		string soundFile1 = argv[12];
		pair<float, float> volume = make_pair(stoi(argv[10]), stoi(argv[11]));

		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);

				Sound<int8_t> sound3 = sound1 * volume.first;
				sound3.saveSound();
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);

				Sound<int16_t> sound3 = sound1 * volume.first;
				sound3.saveSound();
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				Sound<pair<int8_t, int8_t>> sound3 = sound1 * volume;
				sound3.saveSound();
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				Sound<pair<int16_t, int16_t>> sound3 = sound1 * volume;
				sound3.saveSound();
			}
		}
	}
	else if (argument == "-rev")
	{
		string soundFile1 = argv[10];

		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);

				Sound<int8_t> sound3 = sound1.reverse();
				sound3.saveSound();
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);

				Sound<int16_t> sound3 = sound1.reverse();
				sound3.saveSound();
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				Sound<pair<int8_t, int8_t>> sound3 = sound1.reverse();
				sound3.saveSound();
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				Sound<pair<int16_t, int16_t>> sound3 = sound1.reverse();
				sound3.saveSound();
			}
		}
	}
	else if (argument == "-rms")
	{
		string soundFile1 = argv[8];
		cout << "File: " << soundFile1 << "\n";
		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);

				cout << sound1.rms() << "\n";
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);

				cout << sound1.rms() << "\n";
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				pair<float, float> rms = sound1.rms();
				cout << rms.first << " " << rms.second << "\n";
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				pair<float, float> rms = sound1.rms();
				cout << rms.first << " " << rms.second << "\n";
			}
		}
	}
	else if (argument == "-norm")
	{
		string soundFile1 = argv[12];
		pair<float, float> desiredRms = make_pair(stoi(argv[10]), stoi(argv[11]));

		if (channels == 1)
		{
			if (bitCount == 1)
			{
				Sound<int8_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 1, 127);

				Sound<int8_t> sound3 = sound1.normalisation(desiredRms.first);
				sound3.saveSound();
			}
			else
			{
				Sound<int16_t> sound1(sampleRate, soundFile1, outNameFinal, 1, 2, 32767);

				Sound<int16_t> sound3 = sound1.normalisation(desiredRms.first);
				sound3.saveSound();
			}
		}
		else
		{
			if (bitCount == 1)
			{
				Sound<pair<int8_t, int8_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 1, 127);

				Sound<pair<int8_t, int8_t>> sound3 = sound1.normalisation(desiredRms);
				sound3.saveSound();
			}
			else
			{
				Sound<pair<int16_t, int16_t>> sound1(sampleRate, soundFile1, outNameFinal, 2, 2, 32767);

				Sound<pair<int16_t, int16_t>> sound3 = sound1.normalisation(desiredRms);
				sound3.saveSound();
			}
		}
	}
}
