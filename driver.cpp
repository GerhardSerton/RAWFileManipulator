#include <string>
#include <cmath>
#include "sound.h"

using namespace std;
int main(int argc, char * argv[])
{
	int sampleRate = (int)(argv[2]);
	int bitCount = (int)(argv[4]) / 8;
	string bitCountString = argv[4];
	int highValue = ((int)(pow(2.0,(double)bitCount)) / 2) - 1;
	int channels = (int)(argv[6]);
	string channelsString = "MONO";
	if (channels == 2)
	{
		channelsString = "STEREO";
	}
	string outName = "out"
	string argument;
	string soundFile1;
	if (argv[7] == "-o")
	{
		outName = argv[8];
		argument = argv[9];
		soundFile1 = argv[10];
	}
	else
	{
		argument = argv[7];
		soundFile1 = argv[8];
	}
	string outNameFinal;
	outNameFinal << outName << "_" << sampleRate << "_" << bitCountString << "_" << channelsString << ".raw";
	cout << "Outputting to: " << outNameFinal << "\n";

	if (argument == "-add")
	{}
	else if (argument == "-cut")
	{}
	else if (argument == "-radd")
	{}
	else if (argument == "-cat")
	{}
	else if (argument == "-v")
	{}
	else if (argument == "-rev")
	{}
	else if (argument == "-rms")
	{}
	else if (argument == "-norm")
}
