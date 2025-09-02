#include "gbadrumm.h"

//----------VIDEO----------
void loadBg(int bgNum){

    switch(bgNum){
        case 1: //Titlescreen background
            // Load palette
	        memcpy16(pal_bg_mem, backgroundPalettePal, backgroundPalettePalLen / sizeof(u16));
	        // Load Title Screen tiles into CBB 0
	        memcpy32(&tile_mem[0][0], backgroundTitlescreenTiles, backgroundTitlescreenTilesLen / sizeof(u32));
	        // Load Title Screen map into SBB 24
	        memcpy32(&se_mem[24][0], backgroundTitlescreenMap, backgroundTitlescreenMapLen / sizeof(u32));

            //Same for the credits BG
            memcpy32(&tile_mem[1][0], backgroundCreditsTiles, backgroundCreditsTilesLen / sizeof(u32));
            memcpy32(&se_mem[16][0], backgroundCreditsMap, backgroundCreditsMapLen / sizeof(u32));
            break;

        case 2: //Workspace background (the palette is the same as the title screen one)
	        memcpy32(&tile_mem[0][0], backgroundWorkspaceTiles, backgroundWorkspaceTilesLen / sizeof(u32));
	        memcpy32(&se_mem[24][0], backgroundWorkspaceMap, backgroundWorkspaceMapLen / sizeof(u32));
            break;

        default:
            break;
    }

}
void loadSprite(int spriteNum){
    int lastSpriteAddress = 0;
    int objBufferPosition = 0;
    switch(spriteNum){
        case 1: //Titlescreen sprites
            //Palette 0 is shared between all sprites, Palette 1 is used to highlight menu elements that are selected
            memcpy16(pal_obj_mem, titlescreenPalette0Pal, titlescreenPalette0PalLen / 4);
            memcpy16(pal_obj_mem + PALETTE_BANK_OFFSET, titlescreenPalette1Pal, titlescreenPalette1PalLen / 4);

            memcpy32(&tile_mem[4][lastSpriteAddress], logoGba0Tiles, logoGba0TilesLen / 4);
            OBJ_ATTR *logoGba0 = &obj_buffer[objBufferPosition];
            obj_set_attr(logoGba0,
		    ATTR0_SQUARE | ATTR0_BLEND, //Square sprite, alpha channel blending
		    ATTR1_SIZE_16x16, //16x16 pixels
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress); //Palette bank 0, First open position in obj buffer
            lastSpriteAddress += logoGba0TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], logoGba1Tiles, logoGba1TilesLen / 4);
            OBJ_ATTR *logoGba1 = &obj_buffer[objBufferPosition];
            obj_set_attr(logoGba1,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_16x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += logoGba1TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], logoGba2Tiles, logoGba2TilesLen / 4);
            OBJ_ATTR *logoGba2 = &obj_buffer[objBufferPosition];
            obj_set_attr(logoGba2,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_16x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += logoGba2TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], logo0Tiles, logo0TilesLen / 4);
            OBJ_ATTR *logo0 = &obj_buffer[objBufferPosition];
            obj_set_attr(logo0,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_64,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += logo0TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], logo1Tiles, logo1TilesLen / 4);
            OBJ_ATTR *logo1 = &obj_buffer[objBufferPosition];
            obj_set_attr(logo1,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_64,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += logo1TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], logo2Tiles, logo2TilesLen / 4);
            OBJ_ATTR *logo2 = &obj_buffer[objBufferPosition];
            obj_set_attr(logo2,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_64,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += logo2TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNewTiles, textNewTilesLen / 4);
            OBJ_ATTR *textNew = &obj_buffer[objBufferPosition];
            obj_set_attr(textNew,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            lastSpriteAddress += textNewTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textLoadTiles, textLoadTilesLen / 4);
            OBJ_ATTR *textLoad = &obj_buffer[objBufferPosition];
            obj_set_attr(textLoad,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textLoadTilesLen / 32;
            objBufferPosition++;

            //the six "file" sprites are all pointing at the same address in vram, the pointer to the last address is updated only when a new entry is needed
            memcpy32(&tile_mem[4][lastSpriteAddress], textFileTiles, textFileTilesLen / 4);
            OBJ_ATTR *textFile1 = &obj_buffer[objBufferPosition];
            obj_set_attr(textFile1,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *textFile2 = &obj_buffer[objBufferPosition];
            obj_set_attr(textFile2,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *textFile3 = &obj_buffer[objBufferPosition];
            obj_set_attr(textFile3,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *textFile4 = &obj_buffer[objBufferPosition];
            obj_set_attr(textFile4,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *textFile5 = &obj_buffer[objBufferPosition];
            obj_set_attr(textFile5,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *textFile6 = &obj_buffer[objBufferPosition];
            obj_set_attr(textFile6,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textFileTilesLen / 32; 
            objBufferPosition++;

            //same for the empty/full save logo
            memcpy32(&tile_mem[4][lastSpriteAddress], saveLogoTiles, saveLogoTilesLen / 4);
            OBJ_ATTR *saveLogo1 = &obj_buffer[objBufferPosition];
            obj_set_attr(saveLogo1,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *saveLogo2 = &obj_buffer[objBufferPosition];
            obj_set_attr(saveLogo2,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *saveLogo3 = &obj_buffer[objBufferPosition];
            obj_set_attr(saveLogo3,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *saveLogo4 = &obj_buffer[objBufferPosition];
            obj_set_attr(saveLogo4,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *saveLogo5 = &obj_buffer[objBufferPosition];
            obj_set_attr(saveLogo5,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;

            OBJ_ATTR *saveLogo6 = &obj_buffer[objBufferPosition];
            obj_set_attr(saveLogo6,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            objBufferPosition++;
            lastSpriteAddress += saveLogoTilesLen / 32;

            //Load the "saveFull" logo to swap to later, it not loaded into the obj buffer array at present
            memcpy32(&tile_mem[4][lastSpriteAddress], saveLogoFullTiles, saveLogoFullTilesLen / 4);
            lastSpriteAddress += saveLogoFullTilesLen / 32;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNum1Tiles, textNum1TilesLen / 4);
            OBJ_ATTR *textNum1 = &obj_buffer[objBufferPosition];
            obj_set_attr(textNum1,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            lastSpriteAddress += textNum1TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNum2Tiles, textNum2TilesLen / 4);
            OBJ_ATTR *textNum2 = &obj_buffer[objBufferPosition];
            obj_set_attr(textNum2,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textNum2TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNum3Tiles, textNum3TilesLen / 4);
            OBJ_ATTR *textNum3 = &obj_buffer[objBufferPosition];
            obj_set_attr(textNum3,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textNum3TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNum4Tiles, textNum4TilesLen / 4);
            OBJ_ATTR *textNum4 = &obj_buffer[objBufferPosition];
            obj_set_attr(textNum4,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textNum4TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNum5Tiles, textNum5TilesLen / 4);
            OBJ_ATTR *textNum5 = &obj_buffer[objBufferPosition];
            obj_set_attr(textNum5,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textNum5TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNum6Tiles, textNum6TilesLen / 4);
            OBJ_ATTR *textNum6 = &obj_buffer[objBufferPosition];
            obj_set_attr(textNum6,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textNum6TilesLen / 32;
            objBufferPosition++;

            obj_set_pos(logo0, 40, 30); obj_set_pos(logo1, 104, 30); obj_set_pos(logo2, 168, 30);
            obj_set_pos(logoGba0, 157, 51); obj_set_pos(logoGba1, 173, 51); obj_set_pos(logoGba2, 189, 51);
            obj_set_pos(textNew, 108, 80);
            obj_set_pos(textLoad, 104, 100);

            obj_hide_multi(&obj_buffer[8], 18);
            obj_set_pos(saveLogo1, 37, 80); obj_set_pos(textFile1, 53, 80); obj_set_pos(textNum1, 93, 80); obj_set_pos(saveLogo4, 131, 80); obj_set_pos(textFile4, 147, 80); obj_set_pos(textNum4, 187, 80);
            obj_set_pos(saveLogo2, 37, 100); obj_set_pos(textFile2, 53, 100); obj_set_pos(textNum2, 93, 100); obj_set_pos(saveLogo5, 131, 100); obj_set_pos(textFile5, 147, 100); obj_set_pos(textNum5, 187, 100);
            obj_set_pos(saveLogo3, 37, 120); obj_set_pos(textFile3, 53, 120); obj_set_pos(textNum3, 93, 120); obj_set_pos(saveLogo6, 131, 120); obj_set_pos(textFile6, 147, 120); obj_set_pos(textNum6, 187, 120);

            break;

        case 2: //Workspace sprites
            //Load 16 Note objects as unselected notes, the selected alternatives are just loaded in memory and swapped when selected, same with the palettes
            //lastSpriteAddress = 0;
            memcpy16(pal_obj_mem, workspacePalette0Pal, workspacePalette0PalLen / 4);
            memcpy16(pal_obj_mem + PALETTE_BANK_OFFSET, workspacePalette1Pal, workspacePalette1PalLen / 4);
            memcpy16(pal_obj_mem + 2*PALETTE_BANK_OFFSET, workspacePalette2Pal, workspacePalette2PalLen / 4);
            memcpy16(pal_obj_mem + 3*PALETTE_BANK_OFFSET, workspacePalette3Pal, workspacePalette3PalLen / 4);
            memcpy16(pal_obj_mem + 4*PALETTE_BANK_OFFSET, workspacePalette4Pal, workspacePalette4PalLen / 4);

            memcpy32(&tile_mem[4][0], noteUnselectedTiles, noteUnselectedTilesLen / 4);
            for (int i=0; i<16; i++){
                obj_set_attr(&obj_buffer[i],
		        ATTR0_SQUARE | ATTR0_BLEND, //Square sprite with alpha blending
		        ATTR1_SIZE_16x16, // 16x16 pixels,
		        ATTR2_PALBANK(defaultPalette) | 0); // palette index 0, tile index 0
                objBufferPosition++;
            }
            lastSpriteAddress = (noteUnselectedTilesLen / 32);

            OBJ_ATTR *note0 = &obj_buffer[0]; OBJ_ATTR *note1 = &obj_buffer[1]; OBJ_ATTR *note2 = &obj_buffer[2]; OBJ_ATTR *note3 = &obj_buffer[3];
            OBJ_ATTR *note4 = &obj_buffer[4]; OBJ_ATTR *note5 = &obj_buffer[5]; OBJ_ATTR *note6 = &obj_buffer[6]; OBJ_ATTR *note7 = &obj_buffer[7];
            OBJ_ATTR *note8 = &obj_buffer[8]; OBJ_ATTR *note9 = &obj_buffer[9]; OBJ_ATTR *note10 = &obj_buffer[10]; OBJ_ATTR *note11 = &obj_buffer[11];
            OBJ_ATTR *note12 = &obj_buffer[12]; OBJ_ATTR *note13 = &obj_buffer[13]; OBJ_ATTR *note14 = &obj_buffer[14]; OBJ_ATTR *note15 = &obj_buffer[15];

            memcpy32(&tile_mem[4][lastSpriteAddress], noteSelectedTiles, noteSelectedTilesLen / 4);
            lastSpriteAddress += (noteSelectedTilesLen / 32);

            memcpy32(&tile_mem[4][lastSpriteAddress], selectionCursor0Tiles, selectionCursor0TilesLen / 4);
            OBJ_ATTR *selectionCursor = &obj_buffer[objBufferPosition];
            obj_set_attr(selectionCursor,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_16x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += selectionCursor0TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], selectionCursor1Tiles, selectionCursor1TilesLen / 4);
            lastSpriteAddress += selectionCursor1TilesLen / 32;

            memcpy32(&tile_mem[4][lastSpriteAddress], textSaveTiles, textSaveTilesLen / 4);
            OBJ_ATTR *textSave = &obj_buffer[objBufferPosition];
            obj_set_attr(textSave,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textSaveTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textBpmTiles, textBpmTilesLen / 4);
            OBJ_ATTR *textBpm = &obj_buffer[objBufferPosition];
            obj_set_attr(textBpm,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textBpmTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textPlayTiles, textPlayTilesLen / 4);
            OBJ_ATTR *textPlay = &obj_buffer[objBufferPosition];
            obj_set_attr(textPlay,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textPlayTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], buttonLTiles, buttonLTilesLen / 4);
            OBJ_ATTR *buttonL = &obj_buffer[objBufferPosition];
            obj_set_attr(buttonL,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += buttonLTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], buttonRTiles, buttonRTilesLen / 4);
            OBJ_ATTR *buttonR = &obj_buffer[objBufferPosition];
            obj_set_attr(buttonR,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += buttonRTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], arrowUpTiles, arrowUpTilesLen / 4);
            OBJ_ATTR *arrowUp = &obj_buffer[objBufferPosition];
            obj_set_attr(arrowUp,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            lastSpriteAddress += arrowUpTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], arrowDownTiles, arrowDownTilesLen / 4);
            OBJ_ATTR *arrowDown = &obj_buffer[objBufferPosition];
            obj_set_attr(arrowDown,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            lastSpriteAddress += arrowDownTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textQuestionmarkTiles, textQuestionmarkTilesLen / 4);
            OBJ_ATTR *textQuestionmark = &obj_buffer[objBufferPosition];
            obj_set_attr(textQuestionmark,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            lastSpriteAddress += textQuestionmarkTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textYesTiles, textYesTilesLen / 4);
            OBJ_ATTR *textYes = &obj_buffer[objBufferPosition];
            obj_set_attr(textYes,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            lastSpriteAddress += textYesTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], textNoTiles, textNoTilesLen / 4);
            OBJ_ATTR *textNo = &obj_buffer[objBufferPosition];
            obj_set_attr(textNo,
		    ATTR0_WIDE | ATTR0_BLEND,
		    ATTR1_SIZE_8x32,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += textNoTilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], instrument0Tiles, instrument0TilesLen / 4);
            OBJ_ATTR *instrument0 = &obj_buffer[objBufferPosition];
            obj_set_attr(instrument0,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_16x16,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);
            lastSpriteAddress += instrument0TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], instrument1Tiles, instrument1TilesLen / 4);
            OBJ_ATTR *instrument1 = &obj_buffer[objBufferPosition];
            obj_set_attr(instrument1,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_16x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += instrument1TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], instrument2Tiles, instrument2TilesLen / 4);
            OBJ_ATTR *instrument2 = &obj_buffer[objBufferPosition];
            obj_set_attr(instrument2,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_16x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += instrument2TilesLen / 32;
            objBufferPosition++;

            memcpy32(&tile_mem[4][lastSpriteAddress], instrument3Tiles, instrument3TilesLen / 4);
            OBJ_ATTR *instrument3 = &obj_buffer[objBufferPosition];
            obj_set_attr(instrument3,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_16x16,
		    ATTR2_PALBANK(defaultPalette) | lastSpriteAddress);
            lastSpriteAddress += instrument3TilesLen / 32;
            objBufferPosition++;

            //Load all numbers in Vram
            memcpy32(&tile_mem[4][lastSpriteAddress], num0Tiles, num0TilesLen / 4);
            lastSpriteAddress += num0TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num1Tiles, num1TilesLen / 4);
            lastSpriteAddress += num1TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num2Tiles, num2TilesLen / 4);
            lastSpriteAddress += num2TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num3Tiles, num3TilesLen / 4);
            lastSpriteAddress += num3TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num4Tiles, num4TilesLen / 4);
            lastSpriteAddress += num4TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num5Tiles, num5TilesLen / 4);
            lastSpriteAddress += num5TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num6Tiles, num6TilesLen / 4);
            lastSpriteAddress += num6TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num7Tiles, num7TilesLen / 4);
            lastSpriteAddress += num7TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num8Tiles, num8TilesLen / 4);
            lastSpriteAddress += num8TilesLen / 32;
            memcpy32(&tile_mem[4][lastSpriteAddress], num9Tiles, num9TilesLen / 4);
            lastSpriteAddress += num9TilesLen / 32;

            //The position of the BPM's digits are hardcoded in the obj buffer, bad decision but makes the routine to swap between the sprites of the number way easier. Should not interfere with new sprites added
            //I did not add them at the end the obj buffer to avoid having to update the whole array every vblank
            OBJ_ATTR *bpmCount0 = &obj_buffer[31];
            obj_set_attr(bpmCount0,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);

            OBJ_ATTR *bpmCount1 = &obj_buffer[32];
            obj_set_attr(bpmCount1,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);

            OBJ_ATTR *bpmCount2 = &obj_buffer[33];
            obj_set_attr(bpmCount2,
		    ATTR0_SQUARE | ATTR0_BLEND,
		    ATTR1_SIZE_8x8,
		    ATTR2_PALBANK(highlightPalette) | lastSpriteAddress);

            memcpy32(&tile_mem[4][lastSpriteAddress], blankTiles, blankTilesLen / 4);
            lastSpriteAddress += blankTilesLen / 32;

            obj_set_pos(textSave, 24, 11); obj_set_pos(textBpm, 96, 11); obj_set_pos(textPlay, 182, 11);
            obj_set_pos(textQuestionmark, 60, 11); obj_set_pos(textYes, 20, 26); obj_set_pos(textNo, 20, 36);
            obj_set_pos(arrowUp, 136, 3); obj_set_pos(arrowDown, 136, 19);
            obj_set_pos(bpmCount0, 128, 11); obj_set_pos(bpmCount1, 136, 11); obj_set_pos(bpmCount2, 144, 11);
            obj_set_pos(instrument0, 48,40); obj_set_pos(instrument1, 90,40); obj_set_pos(instrument2, 134,40); obj_set_pos(instrument3, 176,40);
            obj_set_pos(buttonL, 16, 45); obj_set_pos(buttonR, 212, 45);

            obj_set_pos(note0, 22, 86); obj_set_pos(note1, 46, 86); obj_set_pos(note2, 70, 86); obj_set_pos(note3, 94, 86);
            obj_set_pos(note4, 132, 86); obj_set_pos(note5, 156, 86); obj_set_pos(note6, 180, 86); obj_set_pos(note7, 204, 86);
            obj_set_pos(note8, 22, 122); obj_set_pos(note9, 46, 122); obj_set_pos(note10, 70, 122); obj_set_pos(note11, 94, 122);
            obj_set_pos(note12, 132, 122); obj_set_pos(note13, 156, 122); obj_set_pos(note14, 180, 122); obj_set_pos(note15, 204, 122);

            obj_set_pos(selectionCursor, 22, 86);
            obj_hide_multi(arrowUp, 5);

            break;

        default:
            break;
    }
}


void setupDisplay(int displayNum){
    switch(displayNum){
        //Opening Background, OPENING_BG
        // Set up BG0, 4bpp, 32x32t map, charblock 0, screenblock 24
        case 1: //Opening screen
	        REG_BG3CNT= BG_CBB(0) | BG_SBB(24) | BG_4BPP | BG_REG_32x32;
            REG_BG2CNT= BG_CBB(1) | BG_SBB(16) | BG_4BPP | BG_REG_32x32;
	        REG_DISPCNT= DCNT_MODE0 | DCNT_BG3 | DCNT_BG2| DCNT_OBJ | DCNT_OBJ_1D;
            oam_copy(oam_mem, obj_buffer, 26);
            break;

        case 2: //Workspace
            REG_BG3CNT= BG_CBB(0) | BG_SBB(24) | BG_4BPP | BG_REG_32x32;
	        REG_DISPCNT= DCNT_MODE0 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;
            break;

        default:
            break;
    }
}

//Functions to swap sprites and palettes. The entries in the obj buffer era rebuilt changing either the palette or the vram tile they point to
void paletteSwap(int spriteNum, int palBank){
    obj_buffer[spriteNum].attr2=ATTR2_BUILD(ATTR2_ID(obj_buffer[spriteNum].attr2), palBank, 0);
}

void spriteSet(int spriteNum, int tileId){
    obj_buffer[spriteNum].attr2=ATTR2_BUILD(tileId, ATTR2_PALBANK(obj_buffer[spriteNum].attr2), 0);
}

/*
NOT USED
void spriteShift(int spriteNum, int tileShift){
    obj_buffer[spriteNum].attr2=ATTR2_BUILD(ATTR2_ID(obj_buffer[spriteNum].attr2) + tileShift, ATTR2_PALBANK(obj_buffer[spriteNum].attr2), 0);
}
*/

//This is probably the worst piece of code in the project. I wrote it after failing to use the tonc text engine. Writes on screen the value of the "num" variable.
//It is almost completly hardcoded, the digit must be in specific position of the obj_buffer (31,32,33, loaded beforehand) and the numbers must be in specific tiles in Vram. If they are changed this function must also be changed
//For this application in particular the flexibility isn't really required but this rigidity is not good at all.
void numberDisplayHandler(int num){
    if (num < 100){
        spriteSet(31, 69); //if the number requires only two digits, then swap the first (obj_buffer[31]) for a blank sprite (in this case tile #69 in Vram)
    }
    int digit = 0;
    while (num != 0){                               //Get the digit the swap the sprite of the digit to the correct number
        spriteSet((33 - digit),(59 + (num % 10))); //Numbers are all loaded sequantially in vram from address #59 onwards. As said before this approach is not flexible but at least should be reasonably fast.
        num = num/10;
        digit++;
    }  
}

//----------AUDIO----------

void soundSetup(){
    //Enabling the sound
    REG_SNDSTAT = SSTAT_ENABLE;
    REG_SNDDMGCNT = MAX_VOLUME_LR | SDMG_LNOISE | SDMG_RNOISE | SDMG_LSQR1 | SDMG_RSQR1;
    REG_SNDDSCNT = SDS_DMG100 | SDS_A100 | SDS_AL | SDS_AR | SDS_ATMR0 | SDS_B100 | SDS_BL | SDS_BR | SDS_BTMR0;

    //Direct Sound setup: Channel A, the kick drum
    REG_DMA1SAD = (unsigned long)sampleKickdrum_bin;
    REG_DMA1DAD= 0x040000A0; //FIFO A Address
    //Direct Sound setup: Channel B, the hi-hat
    REG_DMA2SAD = (unsigned long)sampleHihat_bin;
    REG_DMA2DAD = 0x040000A4; //FIFO B Address
    //Timer setup for the Direct Sound Channels (Shared)
    REG_TM0D = 0xFBE8; //Frequency at which the samples are played (16Khz)
    REG_TM1D = 0xEA18; //Sound lenght (longest of the two) 0xFFFF - 0x(sampleKickdrum) (The longest of the two)

    //Channel 3: Square wave to simulate a bass
    REG_SND1CNT = SSQR_LEN(0) | SSQR_DUTY1_2 | SSQR_TIME(1) | SSQR_DEC | SSQR_IVOL(15);
    REG_SND1SWEEP = SSW_SHIFT(5) | SSW_DEC | SSW_TIME(1);
    
    //Channel 4: Noise channel, snare
    REG_SOUND4CNT_L = CH4_INIT_ENV_VALUE | CH4_ENV_DECREASE | CH4_ENV_STEPTIME | CH4_SOUND_LENGHT;
}