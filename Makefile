CC     := gcc
TARGET := app
HEADIR := inc
SRCDIR := src
OBJDIR := obj
BINDIR := bin
LIBDIR := lib

TESTDIR := tests

EXE := $(BINDIR)/$(TARGET)
SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CFLAGS  := -Wall -O2
INCDIRF := -I$(HEADIR)
LIBDIRF := -L$(LIBDIR)
LIBS    :=

ARGS    :=

$(TARGET): $(EXE)

$(EXE): $(OBJ) | $(BINDIR)
	$(CC) $(LIBDIRF) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIRF) -c $< -o $@

$(BINDIR) $(OBJDIR):
	mkdir -p $@

run: $(TARGET)
	@$(BINDIR)/$(TARGET) $(ARGS)


test: $(wildcard tests/*.c) $(filter-out src/main.c, $(wildcard src/*.c))
	@gcc $^ -o bin/test
	@bin/test

clean:
	@$(RM) -rv $(BINDIR) $(OBJDIR)

.PHONY: all
