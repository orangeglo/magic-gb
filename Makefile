# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = ../../../

LCC = $(GBDK_HOME)bin/lcc 

# You can uncomment the line below to turn on debug output
# LCC = $(LCC) -debug

# You can set the name of the .gb ROM file here
PROJECTNAME    = magic-life

BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c)
ASMSOURCES := $(wildcard src/*.s)

all:	$(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile and link all source files in a single call to LCC
# -Wm-yc : color support
# -Wl-yt0x1B : MBC5 ROM + RAM + Battery (for better compatibility with emulators)
# -Wl-ya1 : 1 RAM bank

$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC) -Wm-yn"MAGICLIFE" -Wl-yt0x1B -Wl-ya1 -D_ram_func=0xD000 -o $@ $(CSOURCES) assets/*.c $(ASMSOURCES)
	python3 rom_size.py $(BINS)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm

dev:
	make && (killall SameBoy || true) && open $(PROJECTNAME).gb