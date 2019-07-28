CC=gcc
LD=ld
#mkfile_path1 = $(abspath $(lastword $(MAKEFILE_LIST)))
#mkfile_path = $(patsubst %/, %, $(dir $(mkfile_path1)))
#change this value to your own source file path
mkfile_path=D:/dev/CorCPP/lua/myluawin

CFLAGS=-I"$(mkfile_path)/thirdparty/lua5.2/include" -mms-bitfields -I"$(mkfile_path)/thirdparty/cairo/include"
CLIBS=-L"$(mkfile_path)/thirdparty/cairo/lib" -lgdi32 -lcairo -lcomdlg32 ./lua52.dll
OBJS=core.o window.o main.o button.o textbox.o cairo.o cairocnt.o listbox.o com.o
MODULES=window\
	button\
	textbox\
	cairo\
	listbox\
	com
OUTNAME=winlua.exe
export CC LD CFLAGS CLIBS 
all:$(MODULES) $(OBJS)
	$(CC)  -mwindows -o $(OUTNAME)  $(OBJS) $(CLIBS)
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
$(MODULES):
	make -C modules\$@
clean:
	rm -f *.o
