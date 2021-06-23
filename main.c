
//**********************************************************************
//*********************** DTMF Dial - Art 2008! ************************
//**********************************************************************

#include <pspkernel.h>
#include <pspiofilemgr.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspaudiolib.h>
#include <pspaudio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <pspgu.h>
#include "graphics.h"
#include "Art.h"
#include "main.h"
#include "dtmf.h"
#include "key.h"
#include "keypad.h"
#include "point.h"
#include "over.h"
#include "click.h"
#include "banner.h"

PSP_MODULE_INFO("DTMF_Dial", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

sample_t* loop_buffer;				//
short mic_level;				//
unsigned int buf[5];				// free memory display buffers
unsigned int *pbuf = buf;			//
char filler[60];				// screen print character buffer
char numbera[1251];				// phone book buffer ( 25 x 50 )
char numbuff[50];				// single phone book record buffer
char numbufp[50];				// single record print buffer
char namebuff[21];				//
char namebufp[21];				//
char keypbuffer[60000];				// buffers for optional external skin
char keytest[60000];				//
int offset;					// phone number buffer offset
int i;						//
int there;					// phone book exists status
int kick = 0;					// kick status outer
int arta = 0x40;				// A
int artr = 0x74;				// r
int artt = 0x74;				// t
int sam = 1;					// sample counter
int samlock = 0;				// player stopped
int samsel = 1;					// sample select
int samend;					// set sample end
int done;					// exit status
int debounce;					//
int skin;					// external skin exists flag
int edit;					// name edit flag
int eflash;					// edit character flash counter
int ey;						// line coordinate
int x;						// emulated mouse pointer coordinates
int y;						//
int pxx;					//
int pyy;					//
int tone;					//
int keyx;					//
int keyy;					//
int pressed;					//
int pset;					//
int numcnt;					//
int namecnt;					//
int ix;						// loop counter
int replay;					// play number flag
int repcnt;					// replay number counter
u32 pixel;					// single pixel colour buffer
u32 sample;					// single pixel colour sample
Image * Art;					// images
Image * gkey;					//
Image * gkeypad;				//
Image * gover;					//
Image * gpoint;					//
Image * gban;					//
Image * test;					//
void clearize(void);				// functions
void prepnum(void);				//
void numcopy(void);				//
void pnumcopy(void);				//
void deleterecord(void);			//
void readbook(void);				//
void savebook(void);				//
void checkbook(void);				//
void copyname(void);				//
void checkskin(void);				//
void checkvalid(void);				//

int exit_callback(int arg1, int arg2, void *common) {
			done = 1;
			sceKernelExitGame();
			return 0;
}

int CallbackThread(SceSize args, void *argp) {
			int cbid;
			cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
			sceKernelRegisterExitCallback(cbid);
			sceKernelSleepThreadCB();
			return 0;
}

int SetupCallbacks(void) {
			int thid = 0;
			thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
			if(thid >= 0) {sceKernelStartThread(thid, 0, 0);}
			return thid;
}
   
void audioOutputLoopCallback(void* buf, unsigned int length, void *userdata) {
			sample_t* ubuf = (sample_t*) buf;
			int i;
			if (samlock == 1) {
			if (samsel == 1) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = dtmf[sam+1] + (dtmf[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++;
			sam++;
			if (sam > samend) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0;
			}
			}
			}
			if (samsel == 2) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			sam++;
			sam++;
			if (sam > samend) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0;
			}
			}
			}
			if (samsel == 3) {
			for (i = 0; i < 1024; i++) {
			ubuf[i].l = click[sam+1] + (click[sam]*256);
			ubuf[i].r = ubuf[i].l;
			sam++;
			sam++;
			if (sam > 2406) {
			ubuf[i].l = 0;
			ubuf[i].r = 0;
			samlock = 0;
			}
			}
			}
			}
			if (samlock == 0) {
			int bbb;
			for (bbb = 0; bbb < 1024; bbb++) {
			ubuf[bbb].l = 0xFFFF;
			ubuf[bbb].r = 0xFFFF;
			}
			}
}

int main(void) {
			tone = 99;
			pressed = 50;
			x = 232;y = 140;
			kick = artr + artt + arta;
			if (kick == 0x128) {
			artr--;
			Artpic[5] = 0x64;
			Artpic[25] = 0x6D;
			Artpic[37] = 0x1D;
			Artpic[12] = 0x27;
			Artpic[7] = 0x64;
			Artpic[26] = 0x6E;
			arta++;
			Artpic[0] = 0xE7;
			Artpic[6] = 0x74;
			artr--;
			Artpic[1] = 0x3E;
			for(i=0; i<316; i++) {Artpic[i] = Artpic[i] ^ 0x6E;}
			Art = loadImageMemory(Artpic, 316);
			gkey = loadImageMemory(key, 254);
			gpoint = loadImageMemory(point, 260);
			gover = loadImageMemory(over, 290);
			gban = loadImageMemory(banner, 997);
			checkskin();
			for (ix=0; ix<30; ix++) {sceDisplayWaitVblankStart();}
			if (skin == 0) {
			gkeypad = loadImageMemory(keypad, 23019);
			} else {
			sprintf(keypbuffer,"./skin.png");
			gkeypad = loadImage(keypbuffer);
			}
			} // kick = 128

			sceDisplayWaitVblankStart();
			sceDisplayWaitVblankStart();
			initGraphics();
			sceDisplayWaitVblankStart();
			sceDisplayWaitVblankStart();
			SetupCallbacks();
			loop_buffer = NULL;
			pspAudioInit();
			sceDisplayWaitVblankStart();
			sceDisplayWaitVblankStart();
			audioLoopStart();
			sceDisplayWaitVblankStart();
			sceDisplayWaitVblankStart();
			clearize();
			prepnum();
			numcopy();

			blitImageToScreen(0, 7, 100, 50, Art, 180,110);	// short intro screen
			sceDisplayWaitVblankStart();
			flipScreen();
			for (ix=0; ix<10; ix++) {sceDisplayWaitVblankStart();}

			blitImageToScreen(0, 7, 100, 50, Art, 180,110);	// short intro screen
			sprintf(filler, "THANKS JASON & JARED FOR MEMORY STICK DONATION!!");
			printTextScreen(50, 42, filler, 0x00EFDDCA);
			sceDisplayWaitVblankStart();
			flipScreen();
			for (ix=0; ix<66; ix++) {sceDisplayWaitVblankStart();}

			blitImageToScreen(0, 7, 100, 50, Art, 180,110);	// short intro screen
			printTextScreen(50, 42, filler, 0x00000000);
			sceDisplayWaitVblankStart();
			flipScreen();

			for (ix=0; ix<6; ix++) {sceDisplayWaitVblankStart();}
			checkbook(); // check for phone book
			for (ix=0; ix<6; ix++) {sceDisplayWaitVblankStart();}

			if (there == 0) {
			blitImageToScreen(0, 7, 100, 50, Art, 180,110);	// short intro screen
			sprintf(filler, "THANKS JASON & JARED FOR MEMORY STICK DONATION!!");
			printTextScreen(50, 42, filler, 0x00000000);
			sprintf(filler, "PHONE BOOK FILE DOES NOT EXIST!");
			printTextScreen(122, 232, filler, 0x00EFDDCA);
			sceDisplayWaitVblankStart();
			flipScreen();
			savebook();
			for (ix=0; ix<80; ix++) {sceDisplayWaitVblankStart();}
			}

			if (there == 1) {
			readbook();
			for (ix=0; ix<80; ix++) {sceDisplayWaitVblankStart();}
			goto startprog;
			}

			checkbook(); // check for phone book second attempt

			if (there == 0) {
			blitImageToScreen(0, 7, 100, 50, Art, 180,110);	// short intro screen
			sprintf(filler, "THANKS JASON & JARED FOR MEMORY STICK DONATION!!");
			printTextScreen(50, 42, filler, 0x00000000);
			sprintf(filler, "PHONE BOOK FILE CREATION FAILED!");
			printTextScreen(122, 232, filler, 0x00EFDDCA);
			sceDisplayWaitVblankStart();
			flipScreen();
			for (ix=0; ix<80; ix++) {sceDisplayWaitVblankStart();}
			done = 1;
			sceKernelExitGame();
			return 0;
			}

			if (there == 1) {
			readbook();
			blitImageToScreen(0, 7, 100, 50, Art, 180,110);	// short intro screen
			sprintf(filler, "THANKS JASON & JARED FOR MEMORY STICK DONATION!!");
			printTextScreen(50, 42, filler, 0x00000000);
			sprintf(filler, "NEW PHONE BOOK HAS BEEN CREATED");
			printTextScreen(122, 232, filler, 0x00EFDDCA);
			sceDisplayWaitVblankStart();
			flipScreen();
			for (ix=0; ix<80; ix++) {sceDisplayWaitVblankStart();}
			goto startprog;
			}

			startprog:
			numcopy();
while(done == 0) {
			blitAlphaImageToScreen(0, 0, 480, 272, gkeypad, 0,0);
			if (skin == 1) {blitAlphaImageToScreen(0, 0, 480, 20, gban, 0,0);}
			blitAlphaImageToScreen(0, 0, 20, 20, gpoint, x,y);
			blitAlphaImageToScreen(0, 0, 272, 20, gover, 188,116);

		//	sprintf(filler, "X:%d Y:%d",x,y);		// debug
		//	printTextScreen(8, 5, filler, 0xFF010101);

			pset = offset;
			if (pset < 9) {
			sprintf(filler, "0%d:",offset + 1);
			} else {
			sprintf(filler, "%d:",offset + 1);
			}
			printTextScreen(203, 116, filler, 0xFFE20202);
			printTextScreen(227, 116, namebuff, 0x0054ED4A);

			if (numbuff[27] == 0x20) {
			printTextScreen(227, 126, numbuff, 0x00EFDDCA);
			} else {
			printTextScreen(203, 126, numbuff, 0x00EFDDCA);
			}

			ey = 136;
			pnumcopy();
			pnumcopy();
			pnumcopy();
			pnumcopy();
			pnumcopy();

			for (ix=0; ix<15; ix++) {
			pset++;
			if (pset == 25) {pset = 0;}
			}

			ey = 36;
			pnumcopy();
			pnumcopy();
			pnumcopy();
			pnumcopy();

			SceCtrlData pad;
			sceCtrlSetSamplingMode(1); 
			sceCtrlPeekBufferPositive(&pad, 1);

			if(pad.Buttons & PSP_CTRL_CROSS) {
			if (debounce > 10) {

			if (x > 71) {
			if (x < 124) {
			if (y > 190) {
			if (y < 244) {
			sam = 1;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 0 sample
			tone = 0x30;
			pressed = 0;
			keyx = 72;
			keyy = 191;
			}}

			if (y > 26) {
			if (y < 80) {
			sam = 163033;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 2 sample
			tone = 0x32;
			pressed = 0;
			keyx = 72;
			keyy = 26;
			}}

			if (y > 80) {
			if (y < 135) {
			sam = 426081;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 5 sample
			tone = 0x35;
			pressed = 0;
			keyx = 72;
			keyy = 81;
			}}

			if (y > 135) {
			if (y < 190) {
			sam = 672129;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 8 sample
			tone = 0x38;
			pressed = 0;
			keyx = 72;
			keyy = 136;
			}}
			}}

			if (x > 18) {
			if (x < 71) {
			if (y > 26) {
			if (y < 80) {
			sam = 82017;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 1 sample
			tone = 0x31;
			pressed = 0;
			keyx = 18;
			keyy = 26;
			}}

			if (y > 80) {
			if (y < 135) {
			sam = 345065;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 4 sample
			tone = 0x34;
			pressed = 0;
			keyx = 18;
			keyy = 81;
			}}

			if (y > 135) {
			if (y < 190) {
			sam = 588113;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 7 sample
			tone = 0x37;
			pressed = 0;
			keyx = 18;
			keyy = 136;
			}}

			if (y > 190) {
			if (y < 244) {
			sam = 839177;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF STAR sample
			tone = 0x2A;
			pressed = 0;
			keyx = 18;
			keyy = 191;
			}}
			}}

			if (x > 124) {
			if (x < 178) {
			if (y > 26) {
			if (y < 80) {
			sam = 244049;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 3 sample
			tone = 0x33;
			pressed = 0;
			keyx = 126;
			keyy = 26;
			}}

			if (y > 80) {
			if (y < 135) {
			sam = 498097;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 6 sample
			tone = 0x36;
			pressed = 0;
			keyx = 126;
			keyy = 81;
			}}

			if (y > 135) {
			if (y < 190) {
			sam = 754145;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 9 sample
			tone = 0x39;
			pressed = 0;
			keyx = 126;
			keyy = 136;
			}}

			if (y > 190) {
			if (y < 244) {
			sam = 950093;		// set sample address counter
			samend = sam +40000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF HASH sample
			tone = 0x23;
			pressed = 0;
			keyx = 126;
			keyy = 191;
			}}
			}}

			if (y > 26) {
			if (y < 244) {
			if (x > 188) {
			if (x < 460) {
			replay = 1;
			repcnt = 0;
			}}}}
			debounce = 0;
			}
			if (edit == 1) {debounce = 7;}
			}

			if (replay == 1) {
			
			if (numbera[repcnt+(offset*50)] == 0x20) {replay = 0;}

			if (numbera[repcnt+(offset*50)] == 0x30) {
			sam = 1;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 0 sample
			pressed = 5;		//
			keyx = 72;		//
			keyy = 191;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x31) {
			sam = 82017;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 1 sample
			pressed = 5;		//
			keyx = 18;		//
			keyy = 26;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x32) {
			sam = 163033;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 2 sample
			pressed = 5;		//
			keyx = 72;		//
			keyy = 26;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x33) {
			sam = 244049;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 3 sample
			pressed = 5;		//
			keyx = 126;		//
			keyy = 26;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x34) {
			sam = 345065;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 4 sample
			pressed = 5;		//
			keyx = 18;		//
			keyy = 81;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x35) {
			sam = 426081;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 5 sample
			pressed = 5;		//
			keyx = 72;		//
			keyy = 81;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x36) {
			sam = 498097;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 6 sample
			pressed = 5;		//
			keyx = 126;		//
			keyy = 81;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x37) {
			sam = 588113;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 7 sample
			pressed = 5;		//
			keyx = 18;		//
			keyy = 136;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x38) {
			sam = 672129;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 8 sample
			pressed = 5;		//
			keyx = 72;		//
			keyy = 136;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x39) {
			sam = 754145;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF 9 sample
			pressed = 5;		//
			keyx = 126;		//
			keyy = 136;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x2A) {
			sam = 839177;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF * sample
			pressed = 5;		//
			keyx = 18;		//
			keyy = 191;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x23) {
			sam = 950093;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 1;		// select DTMF # sample
			pressed = 5;		//
			keyx = 126;		//
			keyy = 191;		//
			}

			if (numbera[repcnt+(offset*50)] == 0x5F) {
			sam = 1;		// set sample address counter
			samend = sam +10000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 2;		// select DTMF SPACE sample
			pressed = 5;		//
			}

			repcnt++;
			} // replay

			if(pad.Buttons & PSP_CTRL_SQUARE) {
			if (debounce > 10) {
			sam = 1;		// set sample address counter
			samend = sam +90000;	// set sample address end
			samlock = 1;		// re-enable player routine
			samsel = 2;		// select DTMF STAR sample
			tone = 0x5F;
			pressed = 0;
			keyx = 18;
			keyy = 191;
			debounce = 0;
			}
			}

			if(pad.Buttons & PSP_CTRL_CIRCLE) {
			if (debounce > 10) {
			if (x > 18) {
			if (x < 178) {
			if (y > 26) {
			if (y < 244) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			deleterecord();		// delete current record
			numcnt = 0;		// reset phone number index
			}}}}

			if (y > 26) {
			if (y < 244) {
			if (x > 188) {
			if (x < 460) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			if (edit == 0) {edit = 1;} else {edit = 0;copyname();namecnt = 0;}
			}}}}
			debounce = 0;		//
			}
			}

			if(pad.Buttons & PSP_CTRL_TRIANGLE) {
			if (debounce > 50) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			sceDisplayWaitVblankStart();
			savebook();
			debounce = 0;
			}
			}

			if(pad.Buttons & PSP_CTRL_START) {}
			if(pad.Buttons & PSP_CTRL_SELECT) {}

			if(pad.Buttons & PSP_CTRL_RTRIGGER) {
			if (debounce > 10) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			offset++;		//
			if (offset > 24) {offset = 0;}
			numcopy();		//
			numcnt = 0;		//
			debounce = 0;		//
			}			//
			}			//

			if(pad.Buttons & PSP_CTRL_LTRIGGER) {
			if (debounce > 10) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			offset--;		//
			if (offset < 0) {offset = 24;}
			numcopy();		//
			numcnt = 0;		//
			debounce = 0;		//
			}			//
			}			//

			if (edit == 0) {
			if(pad.Buttons & PSP_CTRL_DOWN) {if (y < 269) {y++;y++;y++;y++;}}
			if(pad.Buttons & PSP_CTRL_RIGHT) {if (x < 477) {x++;x++;x++;x++;}}
			if(pad.Buttons & PSP_CTRL_UP) {if (y > 3) {y--;y--;y--;y--;}}
			if(pad.Buttons & PSP_CTRL_LEFT) {if (x > 3) {x--;x--;x--;x--;}}
			} else {
			if(pad.Buttons & PSP_CTRL_DOWN) {
			if (debounce > 6) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			if (namebuff[namecnt] > 0x20) {
			namebuff[namecnt]--;	//
			}			//
			debounce = 0;		//
			}
			}
			if(pad.Buttons & PSP_CTRL_UP) {
			if (debounce > 6) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			if (namebuff[namecnt] < 0x7E) {
			namebuff[namecnt]++;	//
			}			//
			debounce = 0;		//
			}
			}
			if(pad.Buttons & PSP_CTRL_LEFT) {
			if (debounce > 6) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			if (namecnt > 0) {namecnt--;}
			debounce = 0;		//
			}
			}
			if(pad.Buttons & PSP_CTRL_RIGHT) {
			if (debounce > 6) {
			sam = 1;		// set sample address counter
			samlock = 1;		// re-enable player routine
			samsel = 3;		// select click sample
			if (namecnt < 16) {namecnt++;}
			debounce = 0;		//
			}
			}
			}

			if(pad.Lx < 20) {if (x > 3) {x--;x--;x--;x--;}}
			if(pad.Lx > 235) {if (x < 477) {x++;x++;x++;x++;}}
			if(pad.Ly > 235) {if (y < 269) {y++;y++;y++;y++;}}
			if(pad.Ly < 20) {if (y > 3) {y--;y--;y--;y--;}}

			if (samsel == 1) {
			if (pressed < 9) {blitAlphaImageToScreen(0, 0, 53, 54, gkey, keyx,keyy);}
			}

			if (replay != 1) {
			if (pressed == 0) {
			if (numcnt < 31) {
			numbera[numcnt+(offset*50)] = tone;
			numcnt++;
			numcopy();
			}
			}
			}

			if (eflash < 6) {
			if (edit == 1) {blitAlphaImageToScreen(0, 0, 7, 8, gkey, 226+(namecnt*8),116);}
			}

			sceDisplayWaitVblankStart();
			flipScreen();

			if (replay == 1) {for (ix=0; ix<22; ix++) {sceDisplayWaitVblankStart();}}

			eflash++; debounce++; pressed++;	// update counters
			if (eflash > 16) {eflash = 0;}
			if (debounce > 998) {debounce = 900;}
			if (pressed > 998) {pressed = 900;}

			} // while
			return 0;
}

void clearize() {
			pyy = 19;
			pxx = 2;
			pixel = getPixelImage(pxx, pyy, gpoint);
			sample = pixel;
			for (pyy=0; pyy<20; pyy++) { // image height
			for (pxx=0; pxx<20; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, gpoint);
			if (pixel == sample) {
			putPixelImage(0x00000000, pxx, pyy, gpoint);
			}
			} // width
			} // height

			for (pyy=0; pyy<54; pyy++) { // image height
			for (pxx=0; pxx<53; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, gkey);
			pixel = pixel - 0x66000000;
			putPixelImage(pixel, pxx, pyy, gkey);
			} // width
			} // height

			for (pyy=0; pyy<20; pyy++) { // image height
			for (pxx=0; pxx<272; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, gover);
			pixel = pixel - 0xEF000000;
			putPixelImage(pixel, pxx, pyy, gover);
			} // width
			} // height

			pyy = 1;
			pxx = 1;
			pixel = getPixelImage(pxx, pyy, Art);
			sample = pixel;
			for (pyy=0; pyy<57; pyy++) { // image height
			for (pxx=0; pxx<100; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, Art);
			if (pixel == sample) {
			putPixelImage(0x00000000, pxx, pyy, Art);
			} else {
			putPixelImage(0x00EFDDCA, pxx, pyy, Art);
			}
			} // width
			} // height

			pyy = 1;
			pxx = 1;
			pixel = getPixelImage(pxx, pyy, gban);
			sample = pixel;
			for (pyy=0; pyy<20; pyy++) { // image height
			for (pxx=0; pxx<480; pxx++) { // image width
			pixel = getPixelImage(pxx, pyy, gban);
			if (pixel == sample) {
			putPixelImage(0x00000000, pxx, pyy, gban);
			}
			} // width
			} // height
}

void prepnum() {
			for (ix=0; ix<1249; ix++) {numbera[ix] = 0x20;}
}

void numcopy() {
			for (ix=0; ix<32; ix++) {numbuff[ix] = numbera[ix+(offset*50)];}
			for (ix=32; ix<49; ix++) {namebuff[ix-32] = numbera[ix+(offset*50)];}
}

void copyname() {
			for (ix=32; ix<49; ix++) {numbera[ix+(offset*50)] = namebuff[ix-32];}
}

void deleterecord() {
			for (ix=0; ix<49; ix++) {numbera[ix+(offset*50)] = 0x20; numbuff[ix] = 0x20;}
			for (ix=32; ix<49; ix++) {namebuff[ix-32] = 0x20;}
}

void readbook() {
			int stg;
			stg = sceIoOpen("./book", PSP_O_RDONLY, 0777);
			sceIoRead(stg,numbera,1251);
			sceIoClose(stg);
}

void savebook() {
			int sstg;
			sstg = sceIoOpen("./book", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
			sceIoWrite(sstg,numbera,1251);
			sceIoClose(sstg);
}

void checkbook() {
			int fdvc;
			fdvc = sceIoOpen("./book", PSP_O_RDONLY, 0); // check existence of phone book
			if(fdvc < 0) {there = 0;} else {there = 1;}
			sceIoClose(fdvc);
}

void checkskin() {
			int skinc;
			skinc = sceIoOpen("./skin.png", PSP_O_RDONLY, 0); // check existence of skin
			if(skinc < 0) {
			skin = 0;
			} else {
			skin = 1;
			sprintf(keytest,"./skin.png");
			test = loadImage(keytest);
			checkvalid();
			}
			sceIoClose(skinc);
}

void pnumcopy() {
			pset++;
			if (pset == 25) {pset = 0;}
			for (ix=0; ix<32; ix++) {
			numbufp[ix] = numbera[ix+(pset*50)];
			}
			for (ix=32; ix<49; ix++) {
			namebufp[ix-32] = numbera[ix+(pset*50)];
			}

			if (pset < 9) {
			sprintf(filler, "0%d:",pset + 1);
			} else {
			sprintf(filler, "%d:",pset + 1);
			}

			printTextScreen(203, ey, filler, 0xFFE20202);
			ey = ey + 10;
			printTextScreen(227, ey-10, namebufp, 0x0054ED4A);

			if (numbuff[27] == 0x20) {
			printTextScreen(227, ey, numbufp, 0x00EFDDCA);
			} else {
			printTextScreen(203, ey, numbufp, 0x00EFDDCA);
			}

			ey = ey + 10;
}

void audioLoopStart() {
			loop_buffer = (sample_t*) malloc(PSP_NUM_AUDIO_SAMPLES * sizeof(sample_t));
			sceKernelDelayThread(200000);	
			pspAudioSetChannelCallback(0, audioOutputLoopCallback, NULL);
}

void checkvalid() {
			pxx = 18;
			pyy = 28;
			pixel = getPixelImage(pxx, pyy, test);
			sample = pixel;
			pixel = getPixelImage(18, 66, test);	// disqualify external skin
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(18, 94, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(18, 160, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(18, 234, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(72, 234, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(70, 213, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(188, 64, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(188, 178, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(188, 238, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(235, 244, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(458, 244, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(396, 26, test);	//
			if (pixel != sample) {skin = 0;}
			pixel = getPixelImage(126, 41, test);	//
			if (pixel != sample) {skin = 0;}
			freeImage(test);			// free image
}


