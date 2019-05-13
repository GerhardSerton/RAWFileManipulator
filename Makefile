#Makefile
#Pieter Gerhard Serton
#27/03/19

CC=g++

Driver: driver.o
	$(CC) -o Driver driver.o

driver.o: driver.cpp
	$(CC) -c -o driver.o driver.cpp

clean:
	@rm -f *.o

debug: driver.o
	$(CC) -g driver.cpp -o debug

debugCatch: catchTests.o
	$(CC) -g catchTests.cpp -o debugCatch

catch: catchTests8BitStereo.o catchTests8BitMono.o catchTests16BitMono.o catchTests16BitStereo.o
	$(CC) -o CatchTests8BitStereo catchTests8BitStereo.o
	$(CC) -o CatchTests16BitStereo catchTests16BitStereo.o
	$(CC) -o CatchTests8BitMono catchTests8BitMono.o
	$(CC) -o CatchTests16BitMono catchTests16BitMono.o

catchTests8BitMono.o: catchTests8BitMono.cpp
	$(CC) -c -o catchTests8BitMono.o catchTests8BitMono.cpp

catchTests8BitStereo.o: catchTests8BitStereo.cpp
	$(CC) -c -o catchTests8BitStereo.o catchTests8BitStereo.cpp

catchTests16BitMono.o: catchTests16BitMono.cpp
	$(CC) -c -o catchTests16BitMono.o catchTests16BitMono.cpp

catchTests16BitStereo.o: catchTests16BitStereo.cpp
	$(CC) -c -o catchTests16BitStereo.o catchTests16BitStereo.cpp
