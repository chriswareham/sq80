VERSION=1.0

CC=cc
CFLAGS=-Wall -Werror $(OPTIM) $(DEBUG)
OPTIM=#-Os
DEBUG=-g -DGTK_DISABLE_SINGLE_INCLUDES -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGSEAL_ENABLE
OBJS=main.o midi.o device.o dialog.o oscillators.o lfos.o filter.o envelopes.o amplifier.o modes.o xmlparser.o
INCS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` -lportmidi

all : sq80

.c.o :
	$(CC) $(CFLAGS) $(INCS) -c $<

sq80 : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

strip : all
	strip sq80

clean : 
	-rm -f *.o *.core sq80

dist : clean
	cd .. && tar cvzf sq80-$(VERSION).tar.gz --exclude CVS sq80

main.o: main.h midi.h dialog.h device.h oscillators.h lfos.h filter.h envelopes.h amplifier.h modes.h xmlparser.h
midi.o: midi.h
device.o: midi.h main.h dialog.h device.h
dialog.o: midi.h main.h dialog.h
oscillators.o: main.h dialog.h oscillators.h
lfos.o: main.h dialog.h lfos.h
filter.o: main.h dialog.h filter.h
envelopes.o: main.h dialog.h envelopes.h
amplifier.o: main.h dialog.h amplifier.h
modes.o: main.h dialog.h modes.h
xmlparser.o: main.h xmlparser.h
