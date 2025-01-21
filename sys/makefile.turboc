ASM     = \borlandc\bin\tasm
AFLAGS  =

TURBO   = \tc\tcc
TFLAGS  = -c -M -mt -S -y -I\tc\include

LINK    = \tc\tlink
LFLAGS  = /T

LIBS    = \tc\lib\cs.lib

INCS    = dos_dd.h

OBJS    = m1.obj m2.obj m3.obj m4.obj m5.obj video.obj com.obj

fujinet.sys: $(OBJS) $(INCS)
	$(LINK) $(LFLAGS) m1+m2+m3+m4+m5+video+com,fujinet.sys,,$(LIBS);

m1.obj: dos_hdr.asm
	copy dos_hdr.asm m1.asm
	$(ASM) $(AFLAGS) m1.asm;

m2.obj: $(INCS) dos_data.c
	$(TURBO) $(TFLAGS) dos_data.c
	arrange dos.arr dos_data.asm m2.asm
	erase dos_data.asm
	$(ASM) $(AFLAGS) m2.asm;

m3.obj: $(INCS) dos_env.c
	$(TURBO) $(TFLAGS) dos_env.c
	arrange dos.arr dos_env.asm m3.asm
	erase dos_env.asm
	$(ASM) $(AFLAGS) m3.asm;

m4.obj: $(INCS) dos_drvr.c
	$(TURBO) $(TFLAGS) dos_drvr.c
	arrange dos.arr dos_drvr.asm m4.asm
	erase dos_drvr.asm
	$(ASM) $(AFLAGS) m4.asm;

m5.obj: $(INCS) dos_end.c
	$(TURBO) $(TFLAGS) dos_end.c
	arrange dos.arr dos_end.asm m5.asm
	erase dos_end.asm
	$(ASM) $(AFLAGS) m5.asm;

video.obj: video.c
	$(TURBO) $(TFLAGS) video.c
	arrange dos.arr video.asm tmp.asm
	erase video.asm
	rename tmp.asm video.asm
	$(ASM) $(AFLAGS) video.asm

com.obj: com.c
	$(TURBO) $(TFLAGS) com.c
	arrange dos.arr com.asm tmp.asm
	erase com.asm
	rename tmp.asm com.asm
	$(ASM) $(AFLAGS) com.asm

clean:
	erase *.OBJ
	erase *.MAP
	erase *.EXE
	erase *.SYS
	erase m1.asm
	erase m2.asm
	erase m3.asm
	erase m4.asm
	erase m5.asm
	erase *.bak
