PSPSDK                 = $(shell psp-config --pspsdk-path)
PSPDIR                 = $(shell psp-config --psp-prefix)

INCDIR                 := ./include
CFLAGS                 = -G0 -Wall -O2 -fno-strict-aliasing
LIBS                   = -lpspaudiolib -lpspaudio -lpspgu -lpsppower -lpng -lz -lm

OBJS                   = main.o graphics.o framebuffer.o

TARGET                 = DTMFDialer
EXTRA_TARGETS          = EBOOT.PBP
PSP_EBOOT_ICON = ICON0.png
PSP_EBOOT_PIC1 = PIC1.PNG
PSP_EBOOT_TITLE        = DTMFDialer
EXTRA_CLEAN            =

include $(PSPSDK)/lib/build.mak

