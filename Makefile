CC=gcc
CFLAGS=-I.
DEPS = globals.h doubleList.h refer.h user.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simonedit: main.o doubleList.o refer.o user.o
	$(CC) -o simonedit.out main.o doubleList.o refer.o user.o