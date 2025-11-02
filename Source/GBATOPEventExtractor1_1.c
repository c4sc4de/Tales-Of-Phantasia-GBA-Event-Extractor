/*
Tales of Phantasia GBA Event Extractor
Written by MagicTablet / c4sc4deb4dge / bjorkfan62 / TwoSpikedHands in 2025
(All the names above are myself, lol)
*/

#include <stdio.h>
#include <stdint.h>

char readFilename[0x255];
char writeFilename[0x255];

int versionOffset = 0xAC;
int gameVersion;

int versionEU = 0x50384E41;
int versionNA = 0x45384E41;
int versionJP = 0x4A384E41;

int pTableStartAll;
int pTableEndAll;

int pTableStartEU = 0x2ADDF0;
int pTableEndEU = 0x2AE228;
int pTableStartNA = 0x21CC38;
int pTableEndNA = 0x21CD0C;
int pTableStartJP = 0x27474C;
int pTableEndJP = 0x274824;

int currentTableOffset;
int currentTableData;
int currentTextTableOffset;

uint8_t currentData;
uint16_t currentTextTablePointer;
int currentTextTableResultOffset;
bool textTableRead = false;

bool completedStop = false;
int timesByteSeen;
bool secondByteSeen = false;


int main() {

    printf("\nUsage:\n\n1. Put your Tales of Phantasia GBA ROM in the same directory as this script.\n2. Enter the filename of the ROM.\n\nThis will create MANY files in this folder.\nEach file is named after the location of the pointer to the event table\nand the actual event table location.\n\n");

    printf("Enter the name of the file: \n\n");
    scanf("%s", readFilename);
    
    FILE *readFile;
    FILE *writeFile;
    readFile = fopen(readFilename, "rb");
    
    if (readFile == NULL) {
        perror("File not found.");
        return 1;
    }

    if (fseek(readFile, versionOffset, SEEK_SET) != 0) {
        perror("Unable to get version offset, maybe wrong file?\n");
        return 2;
    }

    // Verify game version and start gathering data

    if (fread(&gameVersion, sizeof(int), 1, readFile) == 1) {

        if (gameVersion == versionEU) {
            printf("\nRipping events from EU Version.\n\n");

            pTableStartAll = pTableStartEU;
            pTableEndAll = pTableEndEU;

        } else if (gameVersion == versionNA) {
            printf("\nRipping events from NA Version.\n\n");

            pTableStartAll = pTableStartNA;
            pTableEndAll = pTableEndNA;

        } else if (gameVersion == versionJP) {
            printf("\nRipping events from JP Version.\n\n");

            pTableStartAll = pTableStartJP;
            pTableEndAll = pTableEndJP;
            
        }

        fseek(readFile, pTableStartAll, SEEK_SET);
        fread(&currentTableData, sizeof(int), 1, readFile);

        currentTableOffset = pTableStartAll;

        while (currentTableOffset < pTableEndAll) {

            // Grab the offset of the start of the pointer table for events

            fseek(readFile, currentTableOffset, SEEK_SET);
            fread(&currentTableData, sizeof(int), 1, readFile);

            sprintf(writeFilename, "08%x_0%x.bin", currentTableOffset, currentTableData);
            writeFile = fopen(writeFilename, "wb");

            currentTableData -= 0x8000000;

            // Grab the offset of the pointer of the table of the current event map

            fseek(readFile, currentTableData, SEEK_SET);
            fread(&currentData, 1, 1, readFile);
                
            // Determine where the Text Table starts for the current event map
            // Necessary for detecting the ending bytes properly

            currentTextTableOffset = currentTableData + 0x4;

            fseek(readFile, currentTextTableOffset, SEEK_SET);
            fread(&currentTextTablePointer, 2, 1, readFile);

            currentTextTableResultOffset = currentTableData + currentTextTablePointer;

            // Start writing the data to a file until the start of the Text Table

            while (textTableRead == false) {
                fseek(readFile, currentTableData, SEEK_SET);
                fread(&currentData, 1, 1, readFile);
                fwrite(&currentData, 1, 1, writeFile);

                if (currentTableData == currentTextTableResultOffset){
                    textTableRead = true;
                }

                currentTableData++;

            }

            // Once done writing bytes for non-Text Table data, start searching for a sequence of 0x04 0x00 0x00 0x00 0xFF
            // If found, stop writing bytes and start moving to next Event Table

            while (completedStop == false) {

                fseek(readFile, currentTableData, SEEK_SET);
                fread(&currentData, 1, 1, readFile);
                fwrite(&currentData, 1, 1, writeFile);

                if (!(currentData == 0x0 || currentData == 0xFF)) {
                    timesByteSeen = 0;
                    secondByteSeen = false;
                }

                if (secondByteSeen == true) {
                    secondByteSeen = false;
                }

                if (currentData == 0x0) {
                    timesByteSeen += 1;
                }

                if (currentData == 0xFF) {
                    secondByteSeen = true;
                }

                if (timesByteSeen >= 2 && secondByteSeen == true) {
                    completedStop = true;
                }
                    
                currentTableData++;

            }

            completedStop = false;
            textTableRead = false;
            currentTableOffset += 0x04;
            currentTableData = 0;

            fclose(writeFile);

        }

    }

    fclose(readFile);

}

