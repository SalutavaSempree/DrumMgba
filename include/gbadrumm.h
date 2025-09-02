#include <tonc.h>
#include <string.h>

//-----------Title Screen Graphics import-----------
//Palettes
#include "backgroundPalette.h"
#include "titlescreenPalette0.h"
#include "titlescreenPalette1.h"
//Backgrounds
#include "backgroundTitlescreen.h"
#include "backgroundCredits.h"
//Sprites
#include "logo0.h"
#include "logo1.h"
#include "logo2.h"
#include "logoGba0.h"
#include "logoGba1.h"
#include "logoGba2.h"
#include "textNew.h"
#include "textLoad.h"
#include "textFile.h"
#include "textNum1.h"
#include "textNum2.h"
#include "textNum3.h"
#include "textNum4.h"
#include "textNum5.h"
#include "textNum6.h"
#include "saveLogo.h"
#include "saveLogoFull.h"
//-----------Workspace Graphics Import-----------
//Palettes
#include "workspacePalette0.h"
#include "workspacePalette1.h"
#include "workspacePalette2.h"
#include "workspacePalette3.h"
#include "workspacePalette4.h"
//Backgrounds
#include "backgroundWorkspace.h"
//Sprites
#include "noteUnselected.h"
#include "noteSelected.h"
#include "textSave.h"
#include "textBpm.h"
#include "textPlay.h"
#include "buttonL.h"
#include "buttonR.h"
#include "selectionCursor0.h"
#include "selectionCursor1.h"
#include "arrowUp.h"
#include "arrowDown.h"
#include "textQuestionmark.h"
#include "textYes.h"
#include "textNo.h"
#include "instrument0.h"
#include "instrument1.h"
#include "instrument2.h"
#include "instrument3.h"
#include "num0.h"
#include "num1.h"
#include "num2.h"
#include "num3.h"
#include "num4.h"
#include "num5.h"
#include "num6.h"
#include "num7.h"
#include "num8.h"
#include "num9.h"
#include "blank.h"

//-----------Samples data-----------
#include "sampleHihat_bin.h"
#include "sampleKickdrum_bin.h"

//As far as I can tell tonc does not define all of the sound registers settings past the wave channels. Here are ones made by me for the noise channel
/*REG_SOUND4CNT_L, 0x078*/
#define CH4_ENV_INCREASE 0x0800
#define CH4_ENV_DECREASE 0x0000
#define INITIAL_ENV_VALUE100 0xF000

/*REG_SOUND4CNT_H, 0x07C*/
#define CLOCK_DIVIDER_FREQ_2F 0x0000
#define CLOCK_DIVIDER_FREQ_F 0x0001
#define CLOCK_DIVIDER_FREQ_F_OVER_2 0x0002
#define CLOCK_DIVIDER_FREQ_F_OVER_3 0x0003
#define CLOCK_DIVIDER_FREQ_F_OVER_4 0x0004
#define CLOCK_DIVIDER_FREQ_F_OVER_5 0x0005
#define CLOCK_DIVIDER_FREQ_F_OVER_6 0x0006
#define CLOCK_DIVIDER_FREQ_F_OVER_7 0x0007
#define COUNTER_STAGES_7 0X0000
#define COUNTER_STAGES_15 0X0008
#define CH4_MODE_CONTINUOUS 0x0000
#define CH4_MODE_TIMED 0x4000
#define SOUND_RESET 0x8000

//Sound registry defines to tune sound, way less flexible that toncs approach to similar defines but workable
#define CH4_SOUND_LENGHT 0x0000 //Bit 0 to 5
#define CH4_ENV_STEPTIME 0x0100 //Bit Bit 8 to A
#define CH4_INIT_ENV_VALUE  0xD000 // Bit C to F
#define PRESTEPPER_FREQ 0x0000

//Misc defines for some settings/memory offsets
#define PALETTE_BANK_OFFSET 16
#define MAX_VOLUME_LR 0x0077
#define SRAM_OFFSET_32 4
#define SRAM_SAVE_OFFSET 80

extern const int titleScreen;
extern const int workspaceScreen;
extern const int transitionScreen;
extern const int defaultPalette;
extern const int highlightPalette;

extern OBJ_ATTR obj_buffer[];
extern u32 beat[];
extern int bpm;
extern bool playNoteFlag;

extern void loadBg(int bgNum);
extern void loadSprite(int spriteNum);
extern void setupDisplay(int displayNum);
extern void paletteSwap (int spriteNum, int palBank);
extern void spriteSet(int spriteNum, int tileId);
extern void loadSave(int saveNum);
extern void numberDisplayHandler(int num);
extern void soundSetup();
extern void writeSave(int saveNum);
extern int saveCheck(int saveNum);
extern u16 getBpmTableValue(int bpm);
//extern void spriteShift (int spriteNum, int tileShift); NOT USED




