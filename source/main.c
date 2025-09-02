#include "gbadrumm.h"

void noteTimerInterruptHandler(){
    playNoteFlag = 1; //Raise flag
}

void directSoundInterruptHandler(){
    REG_TM0CNT = 0; //Disable timer 0
    REG_DMA1CNT = 0; //Disable DMA 1
    REG_DMA2CNT = 0; //Disable DMA 2
}

int main(void)
{
    int holdSlowdown = 0;
    int selected = 0;
    int choiceCheck = 0;
    int currentFile = 0;

    soundSetup();
    loadBg(titleScreen);
    loadBg(transitionScreen);
    oam_init(obj_buffer, 128);
    loadSprite(titleScreen);

    irq_init(NULL);
	irq_add(II_VBLANK, NULL);
    irq_add(II_TIMER3, noteTimerInterruptHandler);
    irq_add(II_TIMER1, directSoundInterruptHandler);

    setupDisplay(titleScreen);

    //The savefiles with data in them are displayed via a gem-like icon, the check is operated now via the "saveCheck" function
    //This function only checks the first byte of a save where the bpm value is located. If it is different than either 0x00 or 0xFF then data must be present
    for(int i = 0; i < 6; i++){
        u8 temp = 0;
        if((temp = saveCheck(i)) != 0)
            spriteSet(i + 14, 121);
    }

	int xPosBG= 0, yPosBG= 0;

	while(1){
		vid_vsync();
        key_poll();

        if ( (key_hit(KEY_DOWN) || key_hit(KEY_UP)) && selected == 0 ){   //LOAD is now selected
            selected = 1;
            paletteSwap(6,defaultPalette); //sprite #6 in oam buffer (the "NEW" text), with palette bank #0, NEW becomes yellow
            paletteSwap(7,highlightPalette); //sprite #7 in oam buffer (the "LOAD" text), with palette bank #1, LOAD becomes pink

        }
        else if ( (key_hit(KEY_DOWN) || key_hit(KEY_UP)) && selected == 1 ){   //NEW is now selected
            selected = 0;
            paletteSwap(6,highlightPalette); //NEW, pink
            paletteSwap(7,defaultPalette); //LOAD, yellow
        }

        if (key_hit(KEY_A)){
            //Regardless of choice the file selection is displayed. The difference is to either load a savefile or not. this choice is controlled via che chioceCheck variable
            choiceCheck = selected + 1; //Zero means no choice made, one is NEW, otherwise LOAD
            selected = 0;
            obj_unhide_multi(&obj_buffer[8], ATTR0_BLEND, 18); //unhide file selection sprites
            obj_hide_multi (&obj_buffer[6], 2); //Hide NEW and load TEXT
            paletteSwap((selected + 8),highlightPalette); paletteSwap((selected + 14),highlightPalette); paletteSwap((selected + 20),highlightPalette); //ICON, FILE and 1, pink
            while (1){
                vid_vsync();
                key_poll();

                if (key_hit(KEY_DOWN)){
                    paletteSwap((selected + 8),defaultPalette); paletteSwap((selected + 14),defaultPalette); paletteSwap((selected + 20),defaultPalette); //Current entry to default palette
                    selected++;
                    if(selected > 5)
                        selected = 0;
                    paletteSwap((selected + 8),highlightPalette); paletteSwap((selected + 14),highlightPalette); paletteSwap((selected + 20),highlightPalette); //Next entry to highlight palette
                }

                if (key_hit(KEY_UP)){
                    paletteSwap((selected + 8),defaultPalette); paletteSwap((selected + 14),defaultPalette); paletteSwap((selected + 20),defaultPalette); //Current entry to highlight palette
                    selected--;
                    if(selected < 0)
                        selected = 5;
                    paletteSwap((selected + 8),highlightPalette); paletteSwap((selected + 14),highlightPalette); paletteSwap((selected + 20),highlightPalette); //Previous entry to highlight palette
                }

                if (key_hit(KEY_LEFT)){
                    paletteSwap((selected + 8),defaultPalette); paletteSwap((selected + 14),defaultPalette); paletteSwap((selected + 20),defaultPalette); //Current entry to default palette
                    selected -= 3;
                    if (selected < 0)
                        selected += 6;
                    paletteSwap((selected + 8),highlightPalette); paletteSwap((selected + 14),highlightPalette); paletteSwap((selected + 20),highlightPalette); //Previous entry to highlight palette
                }
                
                if(key_hit(KEY_RIGHT)){
                    paletteSwap((selected + 8),defaultPalette); paletteSwap((selected + 14),defaultPalette); paletteSwap((selected + 20),defaultPalette); //Current entry to default palette
                    selected += 3;
                    if (selected > 5)
                        selected -= 6;
                    paletteSwap((selected + 8),highlightPalette); paletteSwap((selected + 14),highlightPalette); paletteSwap((selected + 20),highlightPalette); //Previous entry to highlight palette
                }

                if(key_hit(KEY_B)){ //GO back to new and load text, so hide file selection and unhide those
                    paletteSwap((selected + 8),defaultPalette); paletteSwap((selected + 14),defaultPalette); paletteSwap((selected + 20),defaultPalette); 
                    choiceCheck = 0;
                    selected = 0;
                    obj_hide_multi(&obj_buffer[8], 18);
                    obj_unhide_multi(&obj_buffer[6], ATTR0_BLEND, 2);
                    paletteSwap(6, highlightPalette);
                    paletteSwap(7, defaultPalette);
                    break;
                }

                if (key_hit(KEY_A)){
                    //Save the file selection to know where to save later
                    currentFile = selected;
                    if (choiceCheck == 1){ //NEW was selected from the pevious menu, load blank workspace
                        break;
                    }
                    else{
                        loadSave(selected);
                        break;
                    }
                }
            oam_copy(oam_mem, obj_buffer, 26);
            xPosBG++;
            yPosBG++;
		    REG_BG3HOFS = xPosBG;
		    REG_BG3VOFS = yPosBG;
            }
            if (choiceCheck != 0) //A savefile has been selected, go to workspace
                break;   
        }
        oam_copy(oam_mem, obj_buffer, 26);
        xPosBG++;
        yPosBG++;
		REG_BG3HOFS = xPosBG;
		REG_BG3VOFS = yPosBG;
	}
    
    int animationCount = 0;
    int currentInstrument = 0;
    selected = 0;
    REG_BG3HOFS = 0; REG_BG3VOFS = 0;

    loadBg(workspaceScreen);
    loadSprite(workspaceScreen);

    OBJ_ATTR *selectionCursor = &obj_buffer[16];
    
    setupDisplay(workspaceScreen);
    numberDisplayHandler(bpm);
    oam_copy(oam_mem, obj_buffer, 33);

    //This routine is run everytime the L or R button are pressed to update the beat visuals based od instrument, it is also run now for instrument 0 to display the loaded save
    for (int i=0; i<16; i++){
        if((beat[i] & (0xFF000000)) == (0xFF000000)){
            spriteSet(i, 4);
            paletteSwap(i, highlightPalette);
        }
        else{
            spriteSet(i, 0);
            paletteSwap(i, defaultPalette);
        }
    }

    while(1){
        vid_vsync();
        key_poll();

        if (key_hit(KEY_RIGHT)){
            selected++;
            if (selected > 15)
                selected = 0;
            obj_set_pos(selectionCursor, ATTR1_X(obj_buffer[selected].attr1), ATTR0_Y(obj_buffer[selected].attr0));
        }

        if (key_hit(KEY_LEFT)){
            selected--;
            if (selected < 0)
                selected = 15;
            obj_set_pos(selectionCursor, ATTR1_X(obj_buffer[selected].attr1), ATTR0_Y(obj_buffer[selected].attr0));
        }

        if (key_hit(KEY_DOWN)){
            selected += 8;
            if (selected > 15)
                selected -= 16;
            obj_set_pos(selectionCursor, ATTR1_X(obj_buffer[selected].attr1), ATTR0_Y(obj_buffer[selected].attr0));
        }

        if (key_hit(KEY_UP)){
            selected -= 8;
            if (selected < 0)
                selected += 16;
            obj_set_pos(selectionCursor, ATTR1_X(obj_buffer[selected].attr1), ATTR0_Y(obj_buffer[selected].attr0));
        }

        if (key_hit(KEY_A)){
            if(((beat[selected] ^= (0xFF000000 >> 8*currentInstrument)) & (0xFF000000 >> 8*currentInstrument)) == (0xFF000000 >> 8*currentInstrument)){
                spriteSet(selected, 4); //Change the obj buffer entry to the selected note sprite
                paletteSwap(selected, currentInstrument +1); //Change the palette of the note based on what instrument is currently selected
            }
            else{
                spriteSet(selected, 0);
                paletteSwap(selected, 0);
            }
        }

        if (key_hit(KEY_B)){
            //Erase current instruments notes
            for(int i=0; i < 16; i++){
                beat[i] &= (~(0xFF000000 >> 8*currentInstrument));

                //Update the graphics
                if((beat[i] & (0xFF000000 >> 8*currentInstrument)) == (0xFF000000 >> 8*currentInstrument)){
                spriteSet(i, 4);
                paletteSwap(i, highlightPalette);
                }
                else{
                spriteSet(i, 0);
                paletteSwap(i, defaultPalette);
                }
            }
        }

        //Each time the L or R button are pressed the whole 16 notes need to be update if the next instrument has them selected. This is done via the sprite swap function
        if (key_hit(KEY_L)){
            paletteSwap(currentInstrument + 27, defaultPalette); //Current instrument to default palette
            currentInstrument--;
            if (currentInstrument < 0)
                currentInstrument = 3;
            paletteSwap(currentInstrument + 27, currentInstrument +1); //The currentinstrument variable is used to both determine the sprite to change and to what colour since they are both loaded sequentially
            for (int i=0; i<16; i++){
                //Each element of the beat array is a u32 number. 2bytes are flipped for each instrument to play for a specific note after
                //This way a single element can be masked to know all instruments to be played
                if((beat[i] & (0xFF000000 >> 8*currentInstrument)) == (0xFF000000 >> 8*currentInstrument)){
                    spriteSet(i, 4); //The sprite stays the same, just the tile in vram the sprite points to is changed
                    paletteSwap(i, currentInstrument +1);
                }
                else{
                    spriteSet(i, 0);
                    paletteSwap(i, defaultPalette);
                }
            }

        }

        if (key_hit(KEY_R)){ 
            paletteSwap(currentInstrument + 27, defaultPalette);
            currentInstrument++;
            if (currentInstrument > 3)
                currentInstrument = 0;
            paletteSwap(currentInstrument + 27, currentInstrument +1);
            for (int i=0; i<16; i++){
                if((beat[i] & (0xFF000000 >> 8*currentInstrument)) == (0xFF000000 >> (8)*currentInstrument)){
                    spriteSet(i, 4);
                    paletteSwap(i, currentInstrument +1);
                }
                else{
                    spriteSet(i, 0);
                    paletteSwap(i, defaultPalette);
                }
            }
        }

        if (key_hit(KEY_SELECT)){
            int tempSelected = 0;
            obj_unhide_multi(&obj_buffer[22], ATTR0_BLEND, 2); //Show the arrows
            paletteSwap(18, highlightPalette); paletteSwap(31, highlightPalette); paletteSwap(32, highlightPalette); paletteSwap(33, highlightPalette); //BPM to highlight palette
            while (1){
                vid_vsync();
                key_poll();
                if (key_hit(KEY_LEFT) | key_hit(KEY_RIGHT)){
                    if(tempSelected == 0){
                        tempSelected = 1; //SAVE is selected
                        obj_hide_multi(&obj_buffer[22], 2); //Hide the arrows
                        paletteSwap(17, highlightPalette);  //Highlight save
                        paletteSwap(18, defaultPalette); paletteSwap(31, defaultPalette); paletteSwap(32, defaultPalette); paletteSwap(33, defaultPalette);
                    }
                    else{
                        tempSelected = 0; //LOAD is selected
                        obj_unhide_multi(&obj_buffer[22], ATTR0_BLEND, 2); //Show the arrows
                        paletteSwap(18, highlightPalette); paletteSwap(31, highlightPalette); paletteSwap(32, highlightPalette); paletteSwap(33, highlightPalette); //highlight BPM
                        paletteSwap(17, defaultPalette);
                    }      
                }

                if (key_hit(KEY_A)){ //SAVE has been hit, make the user confirm the choice (or back out)
                    if (tempSelected != 0){
                        obj_unhide_multi(&obj_buffer[24], ATTR0_BLEND, 3); //Show save confirm sprites
                        tempSelected = 0; //Yes (0) is highlighted by default
                        while(1){
                            vid_vsync();
                            key_poll();
                            if (key_hit(KEY_DOWN) | key_hit(KEY_UP)){ //Alternate between selections
                                if (tempSelected == 0){
                                    tempSelected = 1;
                                    paletteSwap(26, highlightPalette); //Highlight No
                                    paletteSwap(25, defaultPalette);
                                }
                                else{
                                    tempSelected = 0;
                                    paletteSwap(25, highlightPalette); //Highlight Yes
                                    paletteSwap(26, defaultPalette);
                                }
                            }

                            if (key_hit(KEY_B)){
                                //Revert to before selecting save and exit from save menu
                                tempSelected = 1;
                                paletteSwap(25, highlightPalette); 
                                paletteSwap(26, defaultPalette);
                                obj_hide_multi (&obj_buffer[24], 3);
                                break;
                            }

                            if (key_hit(KEY_A)){
                                //Save if yes was pressed then revert to before hitting save
                                if(tempSelected == 0){
                                    writeSave(currentFile);
                                }
                                tempSelected = 1;
                                paletteSwap(25, highlightPalette); 
                                paletteSwap(26, defaultPalette);
                                obj_hide_multi (&obj_buffer[24], 3);
                                break;
                            }

                        oam_copy(oam_mem, obj_buffer, 34);
                        }
                    }
                }

                if(tempSelected == 0){

                    while (key_held(KEY_UP)){
                        vid_vsync();
                        key_poll();
                        if (holdSlowdown >= 2){ //Hold down up to scroll bpm fast
                            holdSlowdown = 0;
                            bpm++;
                            if (bpm > 200)
                                bpm = 50;
                            numberDisplayHandler(bpm);
                            paletteSwap(31, highlightPalette); paletteSwap(32, highlightPalette); paletteSwap(33, highlightPalette); //Numbers highlighted
                            oam_copy(oam_mem, obj_buffer, 34);
                            }
                        holdSlowdown++;
                    }

                    if (key_hit(KEY_DOWN)){ //Press down to fine tune the number
                        bpm--;
                        if (bpm < 50)
                            bpm = 200;
                        numberDisplayHandler(bpm);
                        paletteSwap(31, highlightPalette); paletteSwap(32, highlightPalette); paletteSwap(33, highlightPalette);
                    }
                }

                if (key_hit(KEY_SELECT)){
                    obj_hide_multi(&obj_buffer[22], 2); //Hide arrows
                    paletteSwap(17, defaultPalette); //save to default palette
                    paletteSwap(18, defaultPalette); paletteSwap(31, defaultPalette); paletteSwap(32, defaultPalette); paletteSwap(33, defaultPalette); //bpm to default palette
                    break;
                }
                oam_copy(oam_mem, obj_buffer, 34);
            }
        }

        if (key_hit(KEY_START)){
            //Timer 2 overflows every 100 microseconds, cascading into timer 3. When timer 3 overflows it raises an interrupt which flips a flag to play a note the next cycle
            int currentNote = 0; 
            spriteSet(16,8);
            paletteSwap(16,highlightPalette); //Start becomes highlighted
            paletteSwap(19,highlightPalette); //The cursor becomes blue
            oam_copy(oam_mem, obj_buffer, 34);
            //

            //MAKE A FUNCTION
            REG_TM2D = -0x068C; //100microsecons timer
            REG_TM3D = getBpmTableValue(bpm); //LUT for how much time has to pass between notes
            REG_TM3CNT = TM_CASCADE | TM_IRQ | TM_ENABLE; 
            REG_TM2CNT = TM_FREQ_1 | TM_ENABLE;

            while(1){
                bool playDSFlag = 0;
                key_poll();

                if(key_hit(KEY_START)){
                    REG_TM2CNT ^= TM_ENABLE;
                    paletteSwap(19,defaultPalette);
                    paletteSwap(16,defaultPalette);
                    obj_set_pos(selectionCursor, ATTR1_X(obj_buffer[selected].attr1), ATTR0_Y(obj_buffer[selected].attr0));
                    break;
                }

                if(playNoteFlag != 0){
                    //The interrupt has been raised, the flag flipped, time to play the sounds
                    //I wanted to make a function with a switch statment for this but DS1 and DS2 sound channels are related by timers
                    //Timer 1 and the DMA channels are reset here, I do not understand fully why it only works correctly if I do it here. I found this by trial and error
                    //Timer 1 must be reset or the sound freaks out between bpm 70-100
                    REG_TM1CNT = 0;
                    REG_DMA1CNT = 0;
                    REG_DMA2CNT = 0;
                    if((beat[currentNote] & (0xFF000000)) == (0xFF000000)){ //Play instrument 0 sound? (Direct Sound 1, kick drum)
                        //Turn off the DMA (done before) then restart it. This is necessary because if the next sound starts before Timer 1 overflows (The sound lenght counter) then the sound must restart before the interrupt can turn the DMA off
                        REG_SNDDSCNT ^= SDS_ARESET;
                        REG_DMA1CNT = DMA_32 | DMA_AT_FIFO | DMA_REPEAT | DMA_ENABLE;
                        playDSFlag = 1;
                    }

                    if((beat[currentNote] & (0xFF000000 >> 8)) == (0xFF000000 >> (8))){ //Play instrument 1 sound? (Direct Sound 2, hi-hat)
                        //I am genuinely at a loss of words for why this works at all, it should be SDS_BRESET, but that does not work, adding audible popping sounds
                        //This does but should not, after a few wasted hours, and thankful I even found a solution, I'm leaving this as is. If someone finds out or know let me know please
                        REG_SNDDSCNT ^= SDS_ARESET;
                        REG_DMA2CNT = DMA_32 | DMA_AT_FIFO | DMA_REPEAT | DMA_ENABLE;
                        playDSFlag = 1;
                    }
                    //Restart Timer 1, Start timer 0 to play the sound, reset the flag variable
                    //Another flag variable is used here, this operation is performed only if either direct sound channel has been turned on. I do not like this solution very much.
                    if (playDSFlag >= 1){
                        REG_TM1CNT = TM_CASCADE | TM_IRQ | TM_ENABLE;
                        REG_TM0CNT = TM_FREQ_1 | TM_ENABLE;
                        playDSFlag = 0;
                    }

                    if((beat[currentNote] & (0xFF000000 >> 16)) == (0xFF000000 >> (16))){ //Play instrument 2 sound? (bass-like sqaure wave)
                        REG_SND1FREQ = SFREQ_RATE(200) | SFREQ_TIMED | SFREQ_RESET; //70Hz frequency
                    }

                    if((beat[currentNote] & (0xFF000000 >> 24)) == (0xFF000000 >> (24))){ //Play instrument 3 sound? (snare/noise channel)
                        REG_SOUND4CNT_H = CLOCK_DIVIDER_FREQ_2F | COUNTER_STAGES_7 | SOUND_RESET | PRESTEPPER_FREQ | CH4_MODE_TIMED;
                    }

                    obj_set_pos(selectionCursor, ATTR1_X(obj_buffer[currentNote].attr1), ATTR0_Y(obj_buffer[currentNote].attr0));
                    oam_copy(oam_mem, obj_buffer, 33);
                    currentNote++;
                    if (currentNote > 15)
                        currentNote = 0;
                    playNoteFlag = 0;
                }

                oam_copy(oam_mem, obj_buffer, 34);
            }
        }

        //Slows down the cursor animation or it would just flicker
        if (animationCount == 30){
            spriteSet(16,12);
        } else if (animationCount >= 60){
            spriteSet(16,8);
            animationCount = 0;
        }
        animationCount++;
        oam_copy(oam_mem, obj_buffer, 34);
    }
}