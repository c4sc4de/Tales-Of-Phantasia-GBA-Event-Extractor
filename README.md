Usage:

1. Put your Tales of Phantasia GBA ROM in the same directory as this script.
2. Run the script in your terminal using ".\GBATOPExtractor1_1.exe" (Windows) or "./GBATOPExtractor1_1" (Linux)
3. When prompted, enter the filename of the ROM. This will create MANY files in this folder.

Each file is named after the location of the pointer to the event table and the actual event table location.
For example, the event table for Forest of Spirits (Present) in the english language part of the EU version of the game is:

    082addf0_0879a660.bin

These files are intended to be opened with a hex editor.
Each file contains the header, list, scripts, text tables, and text for each event map.

Please note, this is my first REAL project in C, so it's likely got some bugs and is not written well.
From my testing it seems to handle everything perfectly, but the whole point of making this was so I
could learn C and not have to comb through everything byte by byte

If you have any ideas for features or find bugs, let me know on discord, currently bjorkfan62 :D

Version 1.1 -
Fixed a bug where some of the event map tables don't end with the standard ending byte sequence
Updated the script to remove a byte from the ending sequence to conform to all maps