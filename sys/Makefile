AS      = wasm
ASFLAGS = -0 -mt -bt=DOS
CC      = wcc
CFLAGS  = -0 -bt=dos -ms -I.. -s -osh
LD	= wlink
LDFLAGS = SYSTEM dos com ORDER clname SYS_HEADER clname DATA clname CODE clname BSS OPTION MAP

INCS    = dos_dd.h
CFILES  = dos_data.c dos_env.c dos_drvr.c dos_end.c video.c
ASMFILES = dos_hdr.asm
OBJS    = $(CFILES:.c=.o) $(ASMFILES:.asm=.o)

fujinet.sys: $(OBJS) $(INCS)
	$(LD) $(LDFLAGS) \
	  name $@ \
	  library clibc.lib \
	  file $(foreach obj,$(OBJS),$(obj),)
	ls -Fla $@
	hexdump -C $@ | head

dos_env.o: dos_env.c
	$(CC) $(CFLAGS) -nt=_INIT -nc=INIT $<

%.o: %.c
	$(CC) $(CFLAGS) $<
%.o: %.asm
	$(AS) $(ASFLAGS) $<

clean:
	rm -f fujinet.sys *.o
