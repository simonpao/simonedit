CC=gcc
CFLAGS=-I$(IDIR)
IDIR=inc
ODIR=obj
SDIR=src

_DEPS = globals.h doubleList.h refer.h user.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o doubleList.o refer.o user.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

simonedit: $(OBJ)
	$(CC) -o simonedit.out $^ $(CFLAGS) $(LIBS)