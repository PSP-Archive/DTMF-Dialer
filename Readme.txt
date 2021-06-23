;
;`-',
;.-.;""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""";, ,;
;`-';             DTMF Dial V3                  /\    |\            For Sony Playstation Portable   ;`-';
;.-.;                                          //\/''/''/        with Homebrew Compatible Firmware  ;.-.;
;`-';                                         //''||' ||            Last update 14th February '08   ;`-';
;.-.;                                        |/   |/  |/2008!                                       ;.-.;
;`-';                                                                                               ;`-';
;"""'"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""'"""'


Hi Guys,
DTMF Dial is written in C, with the free PSP SDK (thanks to ps2dev.org).


       **************************************** Disclaimer ******************************************


DTMF Dial is provided for free, as it is, without warranty of any kind, either express or
implied, including, but not limited to, the implied warranties of fitness for a purpose.
Without limiting the foregoing, the author of this program makes no warranty that:

   1. the software will meet your requirements
   2. the software will be uninterrupted, or free of errors.
   3. the results that may be obtained from the use of the software will be accurate or reliable.
   4. any errors found in the program will be corrected.
   5. documentation provided with the program will be accurate, complete, or up to date.

Use of this program is at your own discretion and risk and with agreement that you are responsible for any
damages or loss of data that results from such activities.


       ********************************* Grant Of Software License **********************************


DTMF Dial may be free to use, but I retain ownership of my software, and grant of license to use
it is under identical terms of GPS Trackmaker license agreement (reproduced in part with permission),
that pertain to preservation of protected ecosystems. View the entire agreement for GPS Trackmaker:
http://www.gpstm.com/downloadscontract.php

"LICENSE GRANT

The author grants a non-exclusive license to use the program, free of charge, if the user:
Does not use the program for illegal purposes;
does not practice activities that destroy or degrades the environment;
does not practice polluting activities;
does not throw trash on the ground or through the car’s windows;
when going to the beach and creeks takes his trash back until finding a trash collector;
does not practice any activities that hurts animals, like hunting, out-of-season fishing, pigeon shooting,
dog fight, etc.;
does not buy wild animals that, by law, may not be created out of their natural environment;
has ecological conscience and protects nature.
People that for any reason do not fit the conditions above, are expressly forbidden to use the program."

License to use Pointless Pacman, also requires adhesion to the following terms:

When the user visits a Beach or other body of water, National Park or other protected environment,
he or she leaves no damage to live vegetation, leaves no unnatural polluting material, or litter,
and removes non-combustible material from fireplaces, or litter that may have been left by other people
if it is reasonable to store and carry it with your own rubbish.

This agreement applies whether or not the program's source code is ever released to the public.


       ************************************************************************************************


DTMF Dial is a user mode program, and therefore should run on many unofficial firmware variations.
The program is written and tested on DAX 3.03 OE custom firmware.
The eboot directory is run from the GAME150 folder (or the GAME folder for original 1.50 firmware).


       ******************************************* Usage **********************************************


Start the program by selecting it's icon in the XMB game area. The program will show a short splash screen,
and then continue by trying to load an existing saved phonebook file. If the phonebook does not exist,
it will be created by the program, and then flow continues to a keypad input screen. You will also see an
editor window on the right hand side of the screen. This part of the application is not yet fully developed,
as only the selected phone number is shown at any one time.

You can enter phone numbers by simply entering them on the keypad.
Use left and right shoulder buttons to move between the 25 phone number records.
The highlighted area in the middle of the editor window indicates the record that is currently selected.

You can press the CIRCLE button while the pointer is anywhere over the keypad to delete a record if a
mistake was made, or to simply delete a number that will no longer be needed because your girlfriend
dumped you for playing the PSP too much ;)

The SQUARE button is used to insert pauses into the dialing sequence, and this appears on the screen as an
underscore character. A pause may be needed for international dial codes, or to wait for a computerised
voice to speak while auto navigating a tone input based telephone menu system used by many companies.
If you have telephone banking, it should be possible to retrieve your bank balance from your phone without
the need to press any keys on the phone at all!

The TRIANGLE button is used to save all 25 entries of the phone book to a phone book file that is auto
loaded when the program starts up.

The program is exit with the HOME button, but the phone book is not auto saved, so be careful to save the
phonebook first if needed.

To use the keypad in a freestyle fashion, simply press CIRCLE over the keypad to delete a record and start
keying in over it. Just remember not to save the phone book if you were overwriting an important phone number!

To dial a number, move the pointer over the editor window, hold the PSP close to your phone mouthpiece,
and press the CROSS button. Your PSP should be set to high volume, and the PSP speakers should not be close
enough to touch the phone.

A name editor has been introduced into the program with version 2. To enter a name that matches the selected
phone number, press CIRCLE while the pointer is over the editor window. You will see a cursor flash, and the
Dpad will now allow you to enter names. Press the CIRCLE button again to exit editor mode, and remember to
save your settings with the TRIANGLE button when finished. While scrolling through characters, you can speed
things up by holding the CROSS button at the same time as one of the Dpad direction buttons.

While the program operates as normal in editor mode, the name you enter in editor mode is not effective,
even temporarily, until you exit editor mode with the CIRCLE button. You must exit the editor mode before
skipping to the next record, or the name will be lost immediately.


       ************************************* Custom Skin Support **************************************


DTMF Dial V3 introoduces support for custom skins (graphic backgrounds) provided by the end user,
or other users of the program. Some forum members have already created their own. The template provided
in the program package can be used to make your own valid skin file, just edit it in a paint/graphics
program, and save it as "skin.png", and copy it to the program directory. If a valid skin file exists,
it will auomatically be detected and used by the program at startup. If no external skin file is present,
the program will default to the biult in skin graphic. A program routine in this version checks certain
aspects of external skins, and may disqualify them in effort to maintain program functionality.


       ****************************************** Credits *********************************************


Credit where credit is due... It's all mine!!! :) Well, almost. The new default skin was provided by
Jomann of QJ net forums. It looks better than my original, I never claimed to be a graphic artist ;)


       ********************************* 32Mb Memory Stick Donations **********************************


I have valuable use for low capacity Memory Sticks for programming. If you like this program,
please consider donating the 32Mb Memory Stick that came with your PSP once you have upgraded,
or perhaps the low capacity Memory Stick that comes with some Sony Ericsson mobile phones.

Thanks goes out to Jason & Jared, a Father & Son team from QJnet forums for being the first to donate
a pair of Memory Sticks! It's much appreciated! :) Keep up the green electronics projects!
Cheers, Art.

http://forums.ps2dev.org/viewforum.php?f=14
http://forums.qj.net/f-qjnet-sony-psp-forums-48.html/
http://www.dcemu.co.uk/vbulletin/index.php
http://www.psp-programming.com/forums/
http://www.psphacking101.com/forum/index.php?c=1
http://www.austech.info/
http://www.freewebs.com/defxev/


       ************************************ Program History Log ***************************************

VERSION HISTORY:

V3 (14/02/08):
- Added support for an external skin file so the end user can provide their own main program graphic.
- Changed default skin to that provided by Jomann of QJ net forums.
- Various small fixes, including better text formatting in the editor window.

V2 (05/02/08):
- Phone book editor window is now properly implemented as originaly intended.
- Added support for entry of names to match phone numbers in each record.
- Emulated mouse pointer movement is twice as fast.
- Tweaking to tone and pause duration for tone sequence playback.
- Extended pause duration for the pause command that is entered with the SQUARE button.

V1 (04/02/08):
- Basic DTMF dialer can edit and save a small phone book, and playback the DTMF tone sequences for each number.
- Keypad can be used to produce DTMF sequences.


       **********************************************************************************************

