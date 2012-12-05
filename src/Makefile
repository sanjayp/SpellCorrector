PROGRAM = spell_corrector_test
UNITS = spelling_manager

spelling_manager.o: spelling_manager.h
spell_corrector_test.o: spell_corrector_test.cpp

CC = clang++
CFLAGS = -g -Wall -std=c++0x
LDFLAGS = -g

OBJS    = $(UNITS:%=%.o)

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
		$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cpp
		$(CC) -c $(CFLAGS) $<

.PHONY: clean all
	.DEFAULT_GOAL = all

clean:
		rm -f $(OBJS) $(PROGRAM)

