#include "gbadrumm.h"

const int titleScreen = 1;
const int workspaceScreen = 2;
const int transitionScreen = 3;
const int defaultPalette = 0;
const int highlightPalette = 1;

OBJ_ATTR obj_buffer[128];
u32 beat[16] = {0};
int bpm = 60;
bool playNoteFlag = 0;

//The save/load functions do not work on real hardware, i'm sure it's the memcopy fault (and mine for using it), but works on emulator. I will try to make it work in the future
int saveCheck (int saveNum){
    u8 temp = 0;
    memcpy(&temp, sram_mem + saveNum*SRAM_SAVE_OFFSET, sizeof(temp));
    if ((temp != (0x00)) & (temp != (0xFF))) //If different from 00 or FF, so *not* empty, return 1
        return 1;
    else
        return 0;
}

void loadSave(int saveNum){
    
    u8 temp8 = 0;
    //Writing and loading sram only works in 8-bit chunks, requiring nested for loops
    memcpy(&temp8, sram_mem + saveNum*SRAM_SAVE_OFFSET, sizeof(temp8));
    //If there is no save because the user loaded an empty one, then don't load anything
    if ((temp8 == (0x00)) | (temp8 == (0xFF)))
        return;
    bpm = temp8;
    for(int i = 0; i < 16; i++){
        u32 temp32 = 0;
        for(int j = 0; j < 4; j++){
            memcpy(&temp8, sram_mem + saveNum*SRAM_SAVE_OFFSET + (i+1)*SRAM_OFFSET_32 + j, sizeof(temp8));
            temp32 = temp8;
            temp32 = temp32 << 8*j;
            beat[i] |= temp32;
        }
    } 
}

void writeSave(int saveNum){
    
    u8 temp = bpm;
    //Every save is 68 bytes but saves are shifted 80 bytes for data allignment reasons (and because the is plenty of space with saves this small)
    //First 4 bytes are filled by the bpm, memory is padded for data-allignment
    memcpy(sram_mem + saveNum*SRAM_SAVE_OFFSET, &temp, sizeof(temp));
    temp = 0;
    for(int i = 1; i < 4; i++){
        memcpy(sram_mem + saveNum*SRAM_SAVE_OFFSET + i, &temp, sizeof(temp));
    }
    //A loop for every element of the array
    for(int j = 0; j < 16; j++){
        //A loop for every byte of the element
        for(int k = 0; k < 4; k++){
            temp = beat[j] >> 8*k;
            memcpy(sram_mem + saveNum*SRAM_SAVE_OFFSET + (j+1)*SRAM_OFFSET_32 + k, &temp, sizeof(temp));
            }
        }
    //The last 12 bytes to get to 80 bytes are not written to 0
}

//BPM LUT, goes up to 220bpm but only up to 200 is used in the program
const u16 bpmLookupTable[171] = 
{
    0xF448, 0xF483, 0xF4BB, 0xF4F2, 0xF526, 0xF558, 0xF589, 0xF5B8, 0xF5E6, 0xF611,
    0xF63C, 0xF665, 0xF68C, 0xF6B3, 0xF6D8, 0xF6FC, 0xF71F, 0xF741, 0xF762, 0xF782,
    0xF7A1, 0xF7BF, 0xF7DC, 0xF7F9, 0xF815, 0xF830, 0xF84A, 0xF864, 0xF87D, 0xF895,
    0xF8AD, 0xF8C4, 0xF8DA, 0xF8F1, 0xF906, 0xF91B, 0xF930, 0xF944, 0xF957, 0xF96A,
    0xF97D, 0xF98F, 0xF9A1, 0xF9B3, 0xF9C4, 0xF9D5, 0xF9E5, 0xF9F5, 0xFA05, 0xFA15,
    0xFA24, 0xFA33, 0xFA41, 0xFA4F, 0xFA5D, 0xFA6B, 0xFA79, 0xFA86, 0xFA93, 0xFAA0,
    0xFAAC, 0xFAB8, 0xFAC4, 0xFAD0, 0xFADC, 0xFAE7, 0xFAF3, 0xFAFE, 0xFB09, 0xFB13,
    0xFB1E, 0xFB28, 0xFB32, 0xFB3C, 0xFB46, 0xFB50, 0xFB59, 0xFB63, 0xFB6C, 0xFB75,
    0xFB7E, 0xFB87, 0xFB8F, 0xFB98, 0xFBA0, 0xFBA9, 0xFBB1, 0xFBB9, 0xFBC1, 0xFBC9,
    0xFBD0, 0xFBD8, 0xFBDF, 0xFBE7, 0xFBEE, 0xFBF5, 0xFBFC, 0xFC03, 0xFC0A, 0xFC11,
    0xFC18, 0xFC1E, 0xFC25, 0xFC2B, 0xFC32, 0xFC38, 0xFC3E, 0xFC44, 0xFC4A, 0xFC50,
    0xFC56, 0xFC5C, 0xFC62, 0xFC68, 0xFC6D, 0xFC73, 0xFC78, 0xFC7E, 0xFC83, 0xFC88,
    0xFC8D, 0xFC93, 0xFC98, 0xFC9D, 0xFCA2, 0xFCA7, 0xFCAB, 0xFCB0, 0xFCB5, 0xFCBA,
    0xFCBE, 0xFCC3, 0xFCC8, 0xFCCC, 0xFCD1, 0xFCD5, 0xFCD9, 0xFCDE, 0xFCE2, 0xFCE6,
    0xFCEA, 0xFCEE, 0xFCF2, 0xFCF7, 0xFCFB, 0xFCFF, 0xFD02, 0xFD06, 0xFD0A, 0xFD0E,
    0xFD12, 0xFD15, 0xFD19, 0xFD1D, 0xFD20, 0xFD24, 0xFD28, 0xFD2B, 0xFD2F, 0xFD32,
    0xFD35, 0xFD39, 0xFD3C, 0xFD40, 0xFD43, 0xFD46, 0xFD49, 0xFD4D, 0xFD50, 0xFD53, 0xFD56, 
};

u16 getBpmTableValue (int bpm){
    return bpmLookupTable[bpm - 50];
}