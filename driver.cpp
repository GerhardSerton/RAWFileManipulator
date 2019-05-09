#include "sound.h"

int main()
{
	std::cout << "Begin:\n";
  Sound<std::int8_t> test(44100, "8bitmono.raw", "tst.raw", 1, 1, 127);
	test.saveSound();
	return 0;
}
