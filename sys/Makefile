ASM     = wasm
CC      = wcc
#CFLAGS  = -I.. -b dos -fno-stack-check
CFLAGS = -0 -I.. -s -fpc
LINK	= wlink

INCS    = dos_dd.h
CFILES  = dos_data.c dos_env.c dos_drvr.c dos_end.c video.c com.c
ASMFILES = dos_hdr.asm
OBJS    = $(CFILES:.c=.o) $(ASMFILES:.asm=.o)

fujinet.sys: $(OBJS) $(INCS)
	$(LINK) $(LFLAGS) system dos name fujinet.sys library clib.lib file $(foreach obj,$(OBJS),$(obj),)

%.o: %.c
	$(CC) $(CFLAGS) $<
%.o: %.asm
	$(ASM) $(ASFLAGS) $<

clean:
	rm -f fujinet.sys *.o
