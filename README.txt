Pieter Gerhard Serton
.raw file manipulation

To compile this program, simply run "make" to crate an executable named "Driver". To compile the catch tests, run "make catch" instead to create executables named
"CatchTests8BitMono", "CatchTests16BitMono", "CatchTests8BitStereo", and "CatchTests16BitStereo".

"CatchTests" Instructions:
        Simply run an executable to run the tests it contains. Make sure that "8bitmono.raw", "8bitstereo.raw", "16bitmono.raw", and "16bitstereo.raw" are in the
        same directory as the executables, as they are required by the unit tests.
        for the tests.

"Driver" Instructions:
        The structure of the driver commands available is as follows:

        Driver -r [sampleRateInHz] -b [bitCount] -c [noChannels] [-o outFileName ] [<command and it's arguments>] soundFile1 [soundFile2]

        The commands available are:

        -add: add soundFile1 and soundFile2.
        -cut r1 r2: remove samples over range  [r1,r2] (inclusive) (assumes one sound file).
        -radd r1 r2 s1 s2 : add soundFile1 and soundFile2 over sub-ranges indicated (in seconds). The ranges must be equal in length.
        -cat: concatenate soundFile1 and soundFile2•-v r1 r2: volume factor for left/right audio (def=1.0/1.0)(assumes one sound file).
        -rev: reverse sound file (assumes one sound file only).
        -rms: Prints out the RMS of the sound file (assumes one sound file only).
        -norm r1 r2: normalize file for left/right audio (assumes one sound file only and that r1 and r2 are floating point RMS values).
